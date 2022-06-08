// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Constants.

#ifndef ASPS_MODBUS_PDU_SEMANTIC_CONSTANT_HPP
#define ASPS_MODBUS_PDU_SEMANTIC_CONSTANT_HPP

namespace asps {
namespace modbus {
namespace pdu {

enum exception_code {
  exception_code_success = 0x00,
  exception_code_illegal_function = 0x01,
  exception_code_illegal_data_address = 0x02,
  exception_code_illegal_data_value = 0x03,
  exception_code_server_device_failure = 0x04,
  exception_code_acknowledge = 0x05,
  exception_code_service_device_busy = 0x06,
  exception_code_memory_parity_error = 0x08,
  exception_code_gateway_path_unavailable = 0x0a,
  exception_code_gateway_target_device_failed_to_response = 0x0b
};

enum function_code {
  function_code_read_coils = 0x01,
  function_code_read_discrete_inputs = 0x02,
  function_code_read_holding_registers = 0x03,
  function_code_read_input_registers = 0x04,
  function_code_write_single_coil = 0x05,
  function_code_write_single_register = 0x06,
  function_code_write_multiple_coils = 0x0f,
  function_code_write_multiple_registers = 0x10,
  function_code_invalid_pdu = 0x64  // user defined function code
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEMANTIC_CONSTANT_HPP
