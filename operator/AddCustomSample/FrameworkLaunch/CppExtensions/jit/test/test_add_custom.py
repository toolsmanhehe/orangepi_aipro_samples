#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

import os
import shutil
import unittest
import torch
import torch.utils.cpp_extension
import torch_npu
from torch_npu.testing.testcase import TestCase, run_tests

PYTORCH_NPU_INSTALL_PATH = os.path.dirname(os.path.abspath(torch_npu.__file__))


def remove_build_path():
    default_build_root = torch.utils.cpp_extension.get_default_build_root()
    if os.path.exists(default_build_root):
        shutil.rmtree(default_build_root)


class TestCppExtensionsJIT(TestCase):

    def setUp(self):
        super().setUp()
        self.old_working_dir = os.getcwd()
        os.chdir(os.path.dirname(os.path.abspath(__file__)))

    def tearDown(self):
        super().tearDown()
        os.chdir(self.old_working_dir)

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        remove_build_path()

    @classmethod
    def tearDownClass(cls):
        remove_build_path()

    def _test_jit_compile_extension_with_cpp(self):
        extra_ldflags = []
        extra_ldflags.append("-ltorch_npu")
        extra_ldflags.append(f"-L{PYTORCH_NPU_INSTALL_PATH}/lib")
        extra_include_paths = []
        extra_include_paths.append("./")
        extra_include_paths.append(os.path.join(PYTORCH_NPU_INSTALL_PATH, "include"))
        extra_include_paths.append(os.path.join(PYTORCH_NPU_INSTALL_PATH, "include/third_party/acl/inc"))

        module = torch.utils.cpp_extension.load(name="jit_extension",
                                                sources=["../extension_add.cpp"],
                                                extra_include_paths=extra_include_paths,
                                                extra_cflags=["-g"],
                                                extra_ldflags=extra_ldflags,
                                                verbose=True)
        length = [8, 2048]
        x = torch.rand(length, device='cpu', dtype=torch.float16)
        y = torch.rand(length, device='cpu', dtype=torch.float16)
        res = module.add_custom(x.npu(), y.npu())

        self.assertRtolEqual(res.npu(), (x + y))

    def test_jit_compile_extension_with_cpp(self):
        self._test_jit_compile_extension_with_cpp()


if __name__ == '__main__':
    run_tests()
