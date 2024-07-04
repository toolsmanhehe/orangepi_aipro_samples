/**
 * @file extension_add.cpp
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
at::Tensor my_op_impl_npu(const at::Tensor &self, const at::Tensor &other)
{
    // 创建输出内存
    at::Tensor result = at::Tensor(self);

    // 调用aclnn接口计算
    EXEC_NPU_CMD(aclnnAddCustom, self, other, result);
    return result;
}

// 为NPU设备注册反向实现
std::tuple<at::Tensor, at::Tensor> my_op_backward_impl_npu(const at::Tensor &self)
{
    at::Tensor result = at::Tensor(self); // 创建输出内存
    return {result, result};
}

// 为Meta设备注册前向实现
at::Tensor my_op_impl_meta(const at::Tensor &self, const at::Tensor &other)
{
    return empty_like(self);
}

// 为Meta设备注册反向实现
std::tuple<at::Tensor, at::Tensor> my_op_backward_impl_meta(const at::Tensor &self)
{
    auto result = empty_like(self);
    return std::make_tuple(result, result);
}

// 通过继承torch::autograd::Function类实现前反向绑定
class MyAddFunction : public torch::autograd::Function<MyAddFunction> {
public:
    static at::Tensor forward(AutogradContext *ctx, at::Tensor self, at::Tensor other)
    {
        at::AutoDispatchBelowADInplaceOrView guard;
        return my_op_impl_npu(self, other);
    }

    static tensor_list backward(AutogradContext *ctx, tensor_list grad_outputs)
    {
        auto grad_output = grad_outputs[0];
        auto result = my_op_backward_impl_npu(grad_output);
        return {std::get<0>(result), std::get<1>(result)};
    }
};

// 使用的时候调用apply()方法
at::Tensor my_op_impl_autograd(const at::Tensor &self, const at::Tensor &other)
{
    return MyAddFunction::apply(self, other);
}

// 在myops命名空间里注册my_op和my_op_backward两个schema
TORCH_LIBRARY(myops, m)
{
    m.def("my_op(Tensor self, Tensor other) -> Tensor");
    m.def("my_op_backward(Tensor self) -> (Tensor, Tensor)");
}

// 为NPU设备注册前反向实现
// NPU设备在pytorch 2.1及以上版本使用的设备名称是PrivateUse1，在2.1以下版本用的是XLA，如果是2.1以下版本PrivateUse1需要改成XLA
TORCH_LIBRARY_IMPL(myops, PrivateUse1, m)
{
    m.impl("my_op", &my_op_impl_npu);
    m.impl("my_op_backward", &my_op_backward_impl_npu);
}

// 给op绑定NPU的自动求导实现
// 如果是pytorch 2.1以下的版本，AutogradPrivateUse1需要改成AutogradXLA
TORCH_LIBRARY_IMPL(myops, AutogradPrivateUse1, m)
{
    m.impl("my_op", &my_op_impl_autograd);
}

// 为Meta设备注册前反向实现
TORCH_LIBRARY_IMPL(myops, Meta, m)
{
    m.impl("my_op", &my_op_impl_meta);
    m.impl("my_op_backward", &my_op_backward_impl_meta);
}
