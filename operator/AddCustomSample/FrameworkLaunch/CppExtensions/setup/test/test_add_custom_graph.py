#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

from typing import Any, Dict, Iterator, List, Optional, Tuple, Union, Callable
import torch
import torch_npu
from torch_npu.testing.testcase import TestCase, run_tests
import torchair
from torchair.ge_concrete_graph.utils import dtype_promote
from torchair.ge_concrete_graph.fx2ge_converter import register_fx_node_ge_converter
from torchair.ge_concrete_graph.ge_graph import Tensor, TensorSpec
import custom_ops
from auto_generated_ge_raw_custom_ops import AddCustom


@register_fx_node_ge_converter(torch.ops.myops.my_op.default)
def conveter_custom_op(input1: Tensor, input2: Tensor, out: Tensor = None, meta_outputs: Any = None):
    input1, input2 = dtype_promote(input1, input2, target_dtype=meta_outputs.dtype)
    return AddCustom(input1, input2)


class TestCustomAdd(TestCase):

    def test_add_custom_graph(self):

        class PlugInAdd(torch.nn.Module):

            def __init__(self):
                super().__init__()

            def forward(self, input1, input2):
                return torch.ops.myops.my_op(input1, input2)

        length = [8, 2048]
        x = torch.rand(length, device='cpu', dtype=torch.float16)
        y = torch.rand(length, device='cpu', dtype=torch.float16)

        model = PlugInAdd().npu()

        import torchair
        from torchair.configs.compiler_config import CompilerConfig
        config = CompilerConfig()
        npu_backend = torchair.get_npu_backend(compiler_config=config)
        model = torch.compile(model, backend=npu_backend, dynamic=True)

        with torch.no_grad():
            output = model(x.npu(), y.npu())

        cpuout = torch.add(x, y)

        self.assertRtolEqual(output, cpuout)


if __name__ == "__main__":
    run_tests()
