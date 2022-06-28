// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus data model.

#ifndef ASPS_PDU_SEMANTIC_DATA_MODEL_HPP
#define ASPS_PDU_SEMANTIC_DATA_MODEL_HPP

#include <iostream>
#include <cstdint>
#include <vector>

namespace asps {
namespace modbus {
namespace pdu {

// Modbus abstract data type
struct mb_datas
{
  virtual ~mb_datas() {}
  virtual void fill_address(uint16_t address) = 0;
  virtual void remove_fill_data(uint16_t quantity) = 0;
};

// Data model for modbus
// Discrete Input
struct discrete_input
{
  discrete_input()
    : address(0),
      bit(false)
  {}
  discrete_input(uint16_t addr, bool status)
    : address(addr),
      bit(status)
  {}

  uint16_t address;
  const bool bit;
};

struct discrete_inputs
  : public mb_datas,
    public std::vector<discrete_input>
{
  discrete_inputs() = default;
  discrete_inputs(std::initializer_list<discrete_input> il)
    : mb_datas(),
      std::vector<discrete_input>(il)
  {}

  virtual void fill_address(uint16_t address) override;
  virtual void remove_fill_data(uint16_t quantity) override;
};

bool operator==(const discrete_input& first, const discrete_input& second);

// Coil
struct coil
{
  coil()
    : address(0),
      bit(false)
  {}
  coil(uint16_t addr, bool status)
    : address(addr),
      bit(status)
  {}

  uint16_t address;
  bool bit;
};

struct coils
  : public mb_datas,
    public std::vector<coil>
{
  coils() = default;
  coils(std::initializer_list<coil> il)
    : mb_datas(),
      std::vector<coil>(il)
  {}

  virtual void fill_address(uint16_t address) override;
  virtual void remove_fill_data(uint16_t quantity) override;
};

bool operator==(const coil& first, const coil& second);

// Input Register
struct input_register
{
  input_register()
    : address(0),
      word(0)
  {}
  input_register(uint16_t addr, int16_t value)
    : address(addr),
      word(value)
  {}

  uint16_t address;
  const int16_t word;
};

struct input_registers
  : public mb_datas,
    public std::vector<input_register>
{
  input_registers() = default;
  input_registers(std::initializer_list<input_register> il)
    : mb_datas(),
      std::vector<input_register>(il)
  {}

  virtual void fill_address(uint16_t address) override;
  virtual void remove_fill_data(uint16_t quantity) override;
};

bool operator==(const input_register& first, const input_register& second);

// Holding Register
struct holding_register
{
  holding_register()
    : address(0),
      word(0)
  {}
  holding_register(uint16_t addr, int16_t value)
    : address(addr),
      word(value)
  {}

  uint16_t address;
  int16_t word;
};

struct holding_registers :
  public mb_datas,
  public std::vector<holding_register>
{
  holding_registers() = default;
  holding_registers(std::initializer_list<holding_register> il)
    : mb_datas(),
      std::vector<holding_register>(il)
  {}

  virtual void fill_address(uint16_t address) override;
  virtual void remove_fill_data(uint16_t quantity) override;
};

bool operator==(const holding_register& first, const holding_register& second);

} // pdu
} // modbus
} // asps

#endif // ASPS_PDU_SEMANTIC_DATA_MODEL_HPP
