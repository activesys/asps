// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Config.

#ifndef ASPS_MODBUS_API_CONFIG_H
#define ASPS_MODBUS_API_CONFIG_H

#include <cstdint>
#include <algorithm>

namespace asps {
namespace modbus {

// Modbus config
class config
{
public:
  enum limitation {
    max_quantity_of_coils = 0x07d0,
    max_quantity_of_discrete_inputs = 0x07d0,
    max_quantity_of_holding_registers = 0x007d,
    max_quantity_of_input_registers = 0x007d,
    max_quantity_of_write_coils = 0x07b0,
    max_quantity_of_write_registers = 0x007b,
    max_quantity_of_concurrent_requests = 0xffff
  };

public:
  // Quantity for read coils request
  static uint16_t quantity_of_coils()
  {
    return quantity_of_coils_;
  }
  static void quantity_of_coils(uint16_t quantity)
  {
    quantity_of_coils_ =
      std::min<uint16_t>(quantity, max_quantity_of_coils);
  }
  // Quantity for read discrete inputs request
  static uint16_t quantity_of_discrete_inputs()
  {
    return quantity_of_discrete_inputs_;
  }
  static void quantity_of_discrete_inputs(uint16_t quantity)
  {
    quantity_of_discrete_inputs_ =
      std::min<uint16_t>(quantity, max_quantity_of_discrete_inputs);
  }
  // Quantity for read holding registers request
  static uint16_t quantity_of_holding_registers()
  {
    return quantity_of_holding_registers_;
  }
  static void quantity_of_holding_registers(uint16_t quantity)
  {
    quantity_of_holding_registers_ =
      std::min<uint16_t>(quantity, max_quantity_of_holding_registers);
  }
  // Quantity for read input registers request
  static uint16_t quantity_of_input_registers()
  {
    return qunatity_of_input_registers_;
  }
  static void quantity_of_input_registers(uint16_t quantity)
  {
    qunatity_of_input_registers_ =
      std::min<uint16_t>(quantity, max_quantity_of_input_registers);
  }
  // Quantity for write multiple coils request
  static uint16_t quantity_of_write_coils()
  {
    return quantity_of_write_coils_;
  }
  static void quantity_of_write_coils(uint16_t quantity)
  {
    quantity_of_write_coils_ =
      std::min<uint16_t>(quantity, max_quantity_of_write_coils);
  }
  // Quantity for write multiple registers request
  static uint16_t quantity_of_write_registers()
  {
    return quantity_of_write_registers_;
  }
  static void quantity_of_write_registers(uint16_t quantity)
  {
    quantity_of_write_registers_ =
      std::min<uint16_t>(quantity, max_quantity_of_write_registers);
  }
  // Quantity for concurrent requests
  static uint16_t quantity_of_concurrent_requests()
  {
    return quantity_of_concurrent_requests_;
  }
  static void quantity_of_concurrent_requests(uint16_t quantity)
  {
    quantity_of_concurrent_requests_ = quantity;
  }

private:
  static uint16_t quantity_of_coils_;
  static uint16_t quantity_of_discrete_inputs_;
  static uint16_t quantity_of_holding_registers_;
  static uint16_t qunatity_of_input_registers_;
  static uint16_t quantity_of_write_coils_;
  static uint16_t quantity_of_write_registers_;
  static uint16_t quantity_of_concurrent_requests_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_CONFIG_H
