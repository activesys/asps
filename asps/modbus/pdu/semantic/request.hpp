// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Request.

#ifndef ASPS_MODBUS_PDU_SEMANTIC_REQUEST_HPP
#define ASPS_MODBUS_PDU_SEMANTIC_REQUEST_HPP

#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/request_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

enum max_quantity
{
  max_quantity_of_coils = 0x7d0,
  max_quantity_of_inputs = 0x7d0,
  max_quantity_of_registers = 0x007d,
  max_quantity_of_input_registers = 0x007d
};

// Read Coils Request
class read_coils_request
  : public request
{
public:
  read_coils_request(uint16_t starting_address,
                     uint16_t quantity_of_coils)
    : starting_address_(starting_address),
      quantity_of_coils_(quantity_of_coils)
  {
    fc_ = function_code_read_coils;
  }

public:
  virtual bool valid() override;
  virtual request::pointer_type split() override;

public:
  uint16_t address() const
  {return starting_address_;}
  uint16_t quantity() const
  {return quantity_of_coils_;}

private:
  uint16_t starting_address_;
  uint16_t quantity_of_coils_;
};

// Read Discrete Inputs Request
class read_discrete_inputs_request
  : public request
{
public:
  read_discrete_inputs_request(uint16_t starting_address,
                               uint16_t quantity_of_inputs)
    : starting_address_(starting_address),
      quantity_of_inputs_(quantity_of_inputs)
  {
    fc_ = function_code_read_discrete_inputs;
  }

public:
  virtual bool valid() override;
  virtual request::pointer_type split() override;

public:
  uint16_t address() const
  {return starting_address_;}
  uint16_t quantity() const
  {return quantity_of_inputs_;}

private:
  uint16_t starting_address_;
  uint16_t quantity_of_inputs_;
};

// Read Holding Registers Request
class read_holding_registers_request
  : public request
{
public:
  read_holding_registers_request(uint16_t starting_address,
                                 uint16_t quantity_of_registers)
    : starting_address_(starting_address),
      quantity_of_registers_(quantity_of_registers)
  {
    fc_ = function_code_read_holding_registers;
  }

public:
  virtual bool valid() override;
  virtual request::pointer_type split() override;

public:
  uint16_t address() const
  {return starting_address_;}
  uint16_t quantity() const
  {return quantity_of_registers_;}

private:
  uint16_t starting_address_;
  uint16_t quantity_of_registers_;
};

// Read Input Registers Request
class read_input_registers_request
  : public request
{
public:
  read_input_registers_request(uint16_t starting_address,
                               uint16_t quantity_of_registers)
    : starting_address_(starting_address),
      quantity_of_registers_(quantity_of_registers)
  {
    fc_ = function_code_read_input_registers;
  }

public:
  virtual bool valid() override;
  virtual request::pointer_type split() override;

public:
  uint16_t address() const
  {return starting_address_;}
  uint16_t quantity() const
  {return quantity_of_registers_;}

private:
  uint16_t starting_address_;
  uint16_t quantity_of_registers_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEMANTIC_REQUEST_HPP
