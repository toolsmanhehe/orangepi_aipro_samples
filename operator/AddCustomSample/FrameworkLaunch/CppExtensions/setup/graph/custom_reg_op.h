/**
 * @file custom_reg_op.h
 *
 * Copyright (C) 2024. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef ASCENDADAPTER2_CUSTOM_REG_OP_H
#define ASCENDADAPTER2_CUSTOM_REG_OP_H
#include "operator_reg.h"

namespace ge {
REG_OP(AddCustom)
    .INPUT(x, ge::TensorType::ALL())
    .INPUT(y, ge::TensorType::ALL())
    .OUTPUT(z, ge::TensorType::ALL())
    .OP_END_FACTORY_REG(AddCustom);
}

#endif // ASCENDADAPTER2_CUSTOM_REG_OP_H
