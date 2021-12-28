// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Holding Registers and Input Registers.

#ifndef ASPS_MODBUS_REGISTERS_H
#define ASPS_MODBUS_REGISTERS_H

#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace asps {
namespace modbus {

// Modbus Holding Registers
class holding_registers
{
public:
  holding_registers(uint16_t starting_address, uint16_t count, uint16_t* values)
    : starting_address_(starting_address),
      count_(count)
  {
    if (count_ == 0 || values == 0) {
      count_ = 0;
      values_ = 0;
    } else {
      values_ = new uint16_t[count];
      memcpy(values_, values, sizeof(uint16_t) * count);
    }
  }
  ~holding_registers() { delete [] values_; }

  uint16_t operator[](uint16_t address) const
  {
    if (values_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address.");
    }

    return values_[address - starting_address_];
  }

  uint16_t& operator[](uint16_t address)
  {
    if (values_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address.");
    }

    return values_[address - starting_address_];
  }

private:
  uint16_t starting_address_;
  uint16_t count_;
  uint16_t* values_;
};

// Modbus Input Registers
class input_registers
{
public:
  input_registers(uint16_t starting_address, uint16_t count, uint16_t* values)
    : starting_address_(starting_address),
      count_(count)
  {
    if (count_ == 0 || values == 0) {
      count_ = 0;
      values_ = 0;
    } else {
      values_ = new uint16_t[count];
      memcpy(values_, values, sizeof(uint16_t) * count);
    }
  }
  ~input_registers() { delete [] values_; }

  uint16_t operator[](uint16_t address) const
  {
    if (values_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address");
    }

    return values_[address - starting_address_];
  }

private:
  uint16_t starting_address_;
  uint16_t count_;
  uint16_t* values_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_REGISTERS_H
