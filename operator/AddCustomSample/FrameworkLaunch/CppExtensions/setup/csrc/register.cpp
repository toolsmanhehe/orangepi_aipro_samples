/**
 * @file register.cpp
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <torch/extension.h>
#include <torch/library.h>

#include "function.h"

// 在myops命名空间里注册my_op和my_op_backward两个schema
TORCH_LIBRARY(myops, m)
{
    m.def("my_op(Tensor self, Tensor other) -> Tensor");
    m.def("my_op_backward(Tensor self) -> (Tensor, Tensor)");
    m.def("my_op1(Tensor self, Tensor other) -> Tensor");
    m.def("my_op_backward1(Tensor self) -> (Tensor, Tensor)");
}

// 通过pybind将c++接口和python接口绑定
PYBIND11_MODULE(TORCH_EXTENSION_NAME, m)
{
    m.def("add_custom", &my_op_impl_autograd, "x + y");
    m.def("add_custom1", &my_op_impl_autograd1, "x + y");
}
