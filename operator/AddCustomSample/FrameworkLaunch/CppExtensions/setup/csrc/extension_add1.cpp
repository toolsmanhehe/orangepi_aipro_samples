/**
 * @file extension_add1.cpp
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <torch/csrc/autograd/custom_function.h>
#include <torch/library.h>

#include "pytorch_npu_helper.hpp"
using torch::autograd::AutogradContext;
using torch::autograd::Function;
using tensor_list = std::vector<at::Tensor>;
using namespace at;

// 为NPU设备注册前向实现
at::Tensor my_op_impl_npu1(const at::Tensor &self, const at::Tensor &other)
{
    // 创建输出内存
    at::Tensor result = at::Tensor(self);

    // 调用aclnn接口计算
    EXEC_NPU_CMD(aclnnAddCustom, self, other, result);
    return result;
}

// 为NPU设备注册反向实现
std::tuple<at::Tensor, at::Tensor> my_op_backward_impl_npu1(const at::Tensor &self)
{
    at::Tensor result = at::Tensor(self); // 创建输出内存

    return {result, result};
}

// 为Meta设备注册前向实现
at::Tensor my_op_impl_meta1(const at::Tensor &self, const at::Tensor &other)
{
    return empty_like(self);
}

// 为Meta设备注册反向实现
std::tuple<at::Tensor, at::Tensor> my_op_backward_impl_meta1(const at::Tensor &self)
{
    auto result = empty_like(self);
    return std::make_tuple(result, result);
}

// 寻找注册在该op上的不同设备的实现
at::Tensor my_op_impl1(const at::Tensor &self, const at::Tensor &other)
{
    static auto op =
        torch::Dispatcher::singleton().findSchemaOrThrow("myops::my_op1", "").typed<decltype(my_op_impl1)>();
    return op.call(self, other);
}
// 寻找注册在该op上的不同设备的实现
std::tuple<at::Tensor, at::Tensor> my_op_backward_impl1(const at::Tensor &self)
{
    static auto op = torch::Dispatcher::singleton()
                         .findSchemaOrThrow("myops::my_op_backward1", "")
                         .typed<decltype(my_op_backward_impl1)>();
    return op.call(self);
}

// 通过继承torch::autograd::Function类实现前反向绑定
class MyAddFunction1 : public torch::autograd::Function<MyAddFunction1> {
public:
    static at::Tensor forward(AutogradContext *ctx, at::Tensor self, at::Tensor other)
    {
        at::AutoDispatchBelowADInplaceOrView guard;
        return my_op_impl1(self, other);
    }

    static tensor_list backward(AutogradContext *ctx, tensor_list grad_outputs)
    {
        auto grad_output = grad_outputs[0];
        auto result = my_op_backward_impl1(grad_output);
        return {std::get<0>(result), std::get<1>(result)};
    }
};

// 使用的时候调用apply()方法
at::Tensor my_op_impl_autograd1(const at::Tensor &self, const at::Tensor &other)
{
    return MyAddFunction1::apply(self, other);
}

// 为NPU设备注册前反向实现
// NPU设备在pytorch 2.1及以上版本使用的设备名称是PrivateUse1，在2.1以下版本用的是XLA，如果是2.1以下版本PrivateUse1需要改成XLA
TORCH_LIBRARY_IMPL(myops, PrivateUse1, m)
{
    m.impl("my_op1", &my_op_impl_npu1);
    m.impl("my_op_backward1", &my_op_backward_impl_npu1);
}

// 给op绑定NPU的自动求导实现
// 如果是pytorch 2.1以下的版本，AutogradPrivateUse1需要改成AutogradXLA
TORCH_LIBRARY_IMPL(myops, AutogradPrivateUse1, m)
{
    m.impl("my_op1", &my_op_impl_autograd1);
}

// 为Meta设备注册前反向实现
TORCH_LIBRARY_IMPL(myops, Meta, m)
{
    m.impl("my_op1", &my_op_impl_meta1);
    m.impl("my_op_backward1", &my_op_backward_impl_meta1);
}
