#!/usr/bin/python3
# coding=utf-8
#
# Copyright (C) 2023-2024. Huawei Technologies Co., Ltd. All rights reserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# ===============================================================================

import custom_ops_lib


def add_custom(self, other):
    return custom_ops_lib.add_custom(self, other)


def add_custom1(self, other):
    return custom_ops_lib.add_custom1(self, other)
