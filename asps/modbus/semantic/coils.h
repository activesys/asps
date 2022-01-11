// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Coils and Discrete Inputs.

#ifndef ASPS_MODBUS_SEMANTIC_COILS_H
#define ASPS_MODBUS_SEMANTIC_COILS_H

#include <memory>
#include <asps/modbus/semantic/access_model.h>

namespace asps {
namespace modbus {

// Modbus Coils
class coils : public read_write_model<bool>
{
public:
  coils(uint16_t starting_address, uint16_t count, bool* statuses)
    : read_write_model(starting_address, count, statuses)
  {}
};

// Modbus Discrete Input
class discrete_inputs : public read_only_model<bool>
{
public:
  discrete_inputs(
    uint16_t starting_address, uint16_t count, const bool* statuses)
    : read_only_model(starting_address, count, statuses)
  {}
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SEMANTIC_COILS_H
