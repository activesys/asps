// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU.

#ifndef ASPS_MODBUS_PDU_MESSAGE_PDU_H
#define ASPS_MODBUS_PDU_MESSAGE_PDU_H

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

#include <asps/modbus/semantic/coils.h>
#include <asps/modbus/semantic/exception.h>

namespace asps {
namespace modbus {

// Modbus PDU base class
class pdu;
typedef std::shared_ptr<pdu> pdu_ptr;

class pdu
{
protected:
  enum {function_code_field_length = 1};

public:
  typedef std::vector<bool> coils_type;

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

public:
  pdu(uint8_t function_code)
    : function_code_(function_code)
  {}
  virtual ~pdu() {}

public:
  uint8_t function_code() {return function_code_;}

public:
  virtual uint8_t* serialize() = 0;
  virtual std::size_t serialized_size() = 0;

public:
  static pdu_ptr unserialize(const uint8_t* buffer, bool is_request);

protected:
  uint8_t function_code_;
  std::vector<uint8_t> buffer_;
};

// Modbus Exception PDU
class excep_pdu : public pdu
{
  enum {exception_code_field_length = 1};

public:
  enum {exception_id = 0x80};

  excep_pdu(uint8_t function_code, exception_code code)
    : pdu(function_code | exception_id),
      code_(code)
  {}
  ~excep_pdu() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

  exception_code code() {return code_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  exception_code code_;
};

// Modbus Read Coils request PDU
class read_coils_request : public pdu
{
  enum {
    starting_address_field_length = 2,
    quantity_of_coils_field_length = 2
  };

public:
  read_coils_request(uint16_t starting_address, uint16_t quantity_of_coils)
    : pdu(read_coils),
      starting_address_(starting_address),
      quantity_of_coils_(quantity_of_coils)
  {}
  ~read_coils_request() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

  uint16_t starting_address() {return starting_address_;}
  uint16_t quantity_of_coils() {return quantity_of_coils_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  uint16_t starting_address_;
  uint16_t quantity_of_coils_;
};

// Modbus Read Coils Response PDU
class read_coils_response : public pdu
{
  enum {
    byte_count_field_length = 1
  };

public:
  read_coils_response(const coils_type& status)
    : pdu(read_coils),
      coils_status_(status),
      byte_count_((status.size() + 7) / 8)
  {}
  ~read_coils_response() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

  const coils_type& status() {return coils_status_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  coils_type coils_status_;
  uint8_t byte_count_;
};

// Modbus Write Single Coil
class write_single_coil_request : public pdu
{
  enum {
    output_address_field_length = 2,
    output_value_field_length = 2
  };
  enum {
    output_value_on = 0xff00,
    output_value_off = 0x0000
  };

public:
  write_single_coil_request(uint16_t output_address, bool output_value)
    : pdu(write_single_coil),
      output_address_(output_address),
      output_value_(output_value)
  {}
  ~write_single_coil_request() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

public:
  uint16_t output_address() {return output_address_;}
  bool output_value() {return output_value_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  uint16_t output_address_;
  bool output_value_;
};

typedef write_single_coil_request write_single_coil_response;

// Modbus Write Multiple Coils
class write_multiple_coils_request : public pdu
{
  enum {
    starting_address_field_length = 2,
    quantity_of_outputs_field_length = 2,
    byte_count_field_length = 1
  };

public:
  write_multiple_coils_request(
    uint16_t starting_address,
    uint16_t quantity_of_outputs,
    const coils_type& outputs_value)
    : pdu(write_multiple_coils),
      starting_address_(starting_address),
      quantity_of_outputs_(quantity_of_outputs),
      outputs_value_(outputs_value),
      byte_count_((outputs_value.size() + 7)/8)
  {}
  ~write_multiple_coils_request() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

public:
  uint16_t starting_address() {return starting_address_;}
  uint16_t quantity_of_outputs() {return quantity_of_outputs_;}
  const coils_type& outputs_value() {return outputs_value_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  uint16_t starting_address_;
  uint16_t quantity_of_outputs_;
  coils_type outputs_value_;
  uint8_t byte_count_;
};

class write_multiple_coils_response : public pdu
{
  enum {
    starting_address_field_length = 2,
    quantity_of_outputs_field_length = 2
  };

public:
  write_multiple_coils_response(
    uint16_t starting_address, uint16_t quantity_of_outputs)
    : pdu(write_multiple_coils),
      starting_address_(starting_address),
      quantity_of_outputs_(quantity_of_outputs)
  {}
  ~write_multiple_coils_response() {}

public:
  uint8_t* serialize() override;
  std::size_t serialized_size() override;

public:
  uint16_t starting_address() {return starting_address_;}
  uint16_t quantity_of_outputs() {return quantity_of_outputs_;}

public:
  static pdu_ptr unserialize(const uint8_t* buffer);

private:
  uint16_t starting_address_;
  uint16_t quantity_of_outputs_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_PDU_MESSAGE_PDU_H
