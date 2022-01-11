// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU.

#ifndef ASPS_MODBUS_PDU_PDU_H
#define ASPS_MODBUS_PDU_PDU_H

#include <cstdint>

#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

// Modbus PDU base class
class pdu
{
public:
  enum function_codes {
    read_coils = 0x01,
    read_discrete_inputs = 0x02,
    read_holding_registers = 0x03,
    read_input_registers = 0x04,
    write_single_coil = 0x05,
    write_single_register = 0x06,
    write_multiple_coils = 0x0f,
    write_multiple_registers = 0x10
  };

  pdu(uint8_t function_code)
    : function_code_(function_code)
  {}
  virtual ~pdu() {}

  uint8_t function_code() { return function_code_; }

protected:
  uint8_t function_code_;
};

// Modbus Exception PDU
class excep_pdu : public pdu
{
  enum { exception_id = 0x80 };

public:
  enum exception_codes {
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

  excep_pdu(uint8_t function_code, uint8_t exception_code)
    : pdu(function_code | exception_id),
      exception_code_(exception_code)
    {}
  ~excep_pdu() {}

private:
  uint8_t exception_code_;
};

// Modbus Read Coils request PDU
class req_read_coils
  : public pdu
{
public:
  req_read_coils(uint16_t starting_address, uint16_t quantity_of_coils)
    : pdu(read_coils),
      starting_address_(starting_address),
      quantity_of_coils_(quantity_of_coils)
    {}
  ~req_read_coils() {}

private:
  uint8_t function_code_;
  uint16_t starting_address_;
  uint16_t quantity_of_coils_;
};

// Modbus Read Coils Response PDU
class rsp_read_coils
 : public pdu
{
public:
  rsp_read_coils(uint8_t byte_count)
    : pdu(read_coils),
      byte_count_(byte_count)
  {}
  ~rsp_read_coils() {}

private:
  uint8_t byte_count_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_PDU_PDU_H
