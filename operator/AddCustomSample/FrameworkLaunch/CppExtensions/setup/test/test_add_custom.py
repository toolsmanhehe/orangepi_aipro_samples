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
import custom_ops

torch.npu.config.allow_internal_format = False


class TestCustomAdd(TestCase):

    def test_add_custom(self):
        length = [8, 2048]
        x = torch.rand(length, device='cpu', dtype=torch.float16)
        y = torch.rand(length, device='cpu', dtype=torch.float16)

        x_npu = x.npu()
        y_npu = y.npu()
        x_npu.requires_grad = True
        y_npu.requires_grad = True
        output = custom_ops.add_custom(x_npu, y_npu)
        output.backward(output)

        x.requires_grad = True
        y.requires_grad = True
        cpuout = torch.add(x, y)
        cpuout.backward(cpuout)

        self.assertRtolEqual(output, cpuout)
        self.assertRtolEqual(x_npu.grad, x.grad)
        self.assertRtolEqual(y_npu.grad, y.grad)

    def test_add_custom_meta(self):
        length = [8, 2048]
        x = torch.rand(length, device='cpu', dtype=torch.float16)
        y = torch.rand(length, device='cpu', dtype=torch.float16)

        x_input1 = x.to("meta")
        y_input1 = y.to("meta")
        x_input1.requires_grad = True
        y_input1.requires_grad = True
        custom_out = custom_ops.add_custom(x_input1, y_input1)
        custom_out.backward(custom_out)

        x_input2 = x.to("meta")
        y_input2 = y.to("meta")
        x_input2.requires_grad = True
        y_input2.requires_grad = True
        cpuout = torch.add(x_input2, y_input2)
        cpuout.backward(cpuout)

        self.assertTrue(custom_out.is_meta)
        self.assertRtolEqual(custom_out.size(), cpuout.size())
        self.assertRtolEqual(x_input1.grad.size(), x_input2.grad.size())
        self.assertRtolEqual(y_input1.grad.size(), y_input2.grad.size())


if __name__ == "__main__":
    run_tests()
