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
import glob
import torch
from setuptools import setup, find_packages
from torch.utils.cpp_extension import BuildExtension

import torch_npu
from torch_npu.utils.cpp_extension import NpuExtension

PYTORCH_NPU_INSTALL_PATH = os.path.dirname(os.path.abspath(torch_npu.__file__))
source_files = glob.glob(os.path.join("./csrc/", "*.cpp"))
USE_NINJA = os.getenv('USE_NINJA') == '1'

exts = []
ext1 = NpuExtension(
    name="custom_ops_lib",
    # 如果还有其他cpp文件参与编译，需要在这里添加
    sources=source_files,
    extra_compile_args=[
        '-I' +
        os.path.join(PYTORCH_NPU_INSTALL_PATH, "include/third_party/acl/inc"),
    ],
)
exts.append(ext1)

setup(
    name="custom_ops",
    version='1.0',
    keywords='custom_ops',
    ext_modules=exts,
    packages=find_packages(),
    cmdclass={"build_ext": BuildExtension.with_options(use_ninja=USE_NINJA)},
)
