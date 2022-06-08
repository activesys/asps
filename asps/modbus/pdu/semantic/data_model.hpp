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
  uint16_t address;
  const bool bit;
};

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

struct coils : public mb_datas, public std::vector<coil>
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
bool operator==(const coils& first, const coils& second);

// Input Register
struct input_register
{
  uint16_t address;
  const int16_t word;
};

// Holding Register
struct holding_register
{
  uint16_t address;
  int16_t word;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_PDU_SEMANTIC_DATA_MODEL_HPP
