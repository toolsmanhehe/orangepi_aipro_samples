#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

import torch
import torch_npu
from torch_npu.testing.testcase import TestCase, run_tests

torch.npu.config.allow_internal_format = False

torch.ops.load_library("../build/libAddCustom.so")


class TestCustomAdd(TestCase):

    def test_add_custom_ops(self):
        length = [8, 2048]
        x = torch.rand(length, device='cpu', dtype=torch.float16)
        y = torch.rand(length, device='cpu', dtype=torch.float16)

        x_npu = x.npu()
        y_npu = y.npu()
        x_npu.requires_grad = True
        y_npu.requires_grad = True
        output = torch.ops.myops.my_op(x_npu, y_npu)
        output.backward(output)

        x.requires_grad = True
        y.requires_grad = True
        cpuout = torch.add(x, y)
        cpuout.backward(cpuout)

        self.assertRtolEqual(output, cpuout)
        self.assertRtolEqual(x_npu.grad, x.grad)
        self.assertRtolEqual(y_npu.grad, y.grad)


if __name__ == "__main__":
    run_tests()
