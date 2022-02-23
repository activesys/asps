// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Constants.

#ifndef ASPS_MODBUS_SEMANTIC_CONSTANT_H
#define ASPS_MODBUS_SEMANTIC_CONSTANT_H

#include <cstdint>
#include <vector>

namespace asps {
namespace modbus {

enum exception_code {
  success = 0x00,
  illegal_function = 0x01,
  illegal_data_address = 0x02,
  illegal_data_value = 0x03,
  server_device_failure = 0x04,
  acknowledge = 0x05,
  service_device_busy = 0x06,
  memory_parity_error = 0x08,
  gateway_path_unavailable = 0x0a,
  gateway_target_device_failed_to_response = 0x0b
};

enum function_codes {
  read_coils = 0x01,
  read_discrete_inputs = 0x02,
  read_holding_registers = 0x03,
  read_input_registers = 0x04,
  write_single_coil = 0x05,
  write_single_register = 0x06,
  write_multiple_coils = 0x0f,
  write_multiple_registers = 0x10,
  invalid_pdu = 0x64  // user defined function code
};

typedef bool bit_type;
typedef std::vector<bit_type> bits_type;
typedef uint16_t word_type;
typedef std::vector<word_type> words_type;


} // modbus
} // asps

#endif // ASPS_MODBUS_SEMANTIC_CONSTANT_H
