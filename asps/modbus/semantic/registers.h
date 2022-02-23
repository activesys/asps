// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Holding Registers and Input Registers.

#ifndef ASPS_MODBUS_SEMANTIC_REGISTERS_H
#define ASPS_MODBUS_SEMANTIC_REGISTERS_H

#if 0
#include <asps/modbus/semantic/access_model.h>

namespace asps {
namespace modbus {

// Modbus Holding Registers
class holding_registers : public read_write_model<uint16_t>
{
public:
  holding_registers(
    uint16_t starting_address, uint16_t count, uint16_t* values)
    : read_write_model(starting_address, count, values)
  {}
};

// Modbus Input Registers
class input_registers : public read_only_model<uint16_t>
{
public:
  input_registers(
    uint16_t starting_address, uint16_t count, const uint16_t* values)
    : read_only_model(starting_address, count, values)
  {}
};

} // namespace modbus
} // namespace asps

#endif

#endif // ASPS_MODBUS_SEMANTIC_REGISTERS_H
