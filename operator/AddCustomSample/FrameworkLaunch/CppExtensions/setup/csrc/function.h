/**
 * @file function.h
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <ATen/ATen.h>

at::Tensor my_op_impl_autograd(const at::Tensor &self, const at::Tensor &other);
at::Tensor my_op_impl_autograd1(const at::Tensor &self, const at::Tensor &other);

#endif //  FUNCTION_H_
