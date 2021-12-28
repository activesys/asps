// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Coils and Discrete Inputs.

#ifndef ASPS_MODBUS_COILS_H
#define ASPS_MODBUS_COILS_H

#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace asps {
namespace modbus {

// Modbus Coils
class coils
{
public:
  coils(uint16_t starting_address, uint16_t count, bool* statuses)
    : starting_address_(starting_address),
      count_(count)
  {
    if (count_ == 0 || statuses == 0) {
      count_ = 0;
      statuses_ = 0;
    } else {
      statuses_ = new bool[count];
      memcpy(statuses_, statuses, sizeof(bool) * count);
    }
  }
  ~coils() { delete [] statuses_; }

  bool operator[](uint16_t address) const
  {
    if (statuses_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address.");
    }

    return statuses_[address - starting_address_];
  }

  bool& operator[](uint16_t address)
  {
    if (statuses_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address.");
    }

    return statuses_[address - starting_address_];
  }

private:
  uint16_t starting_address_;
  uint16_t count_;
  bool* statuses_;
};

// Modbus Discrete Input
class discrete_inputs
{
public:
  discrete_inputs(uint16_t starting_address, uint16_t count, bool* statuses)
  : starting_address_(starting_address),
    count_(count)
  {
    if (count_ == 0 || statuses == 0) {
      count_ = 0;
      statuses_ = 0;
    } else {
      statuses_ = new bool [count];
      memcpy(statuses_, statuses, sizeof(bool) * count);
    }
  }
  ~discrete_inputs() { delete [] statuses_; }

  bool operator[](uint16_t address) const
  {
    if (statuses_ == 0 ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address.");
    }

    return statuses_[address - starting_address_];
  }

private:
  uint16_t starting_address_;
  uint16_t count_;
  bool* statuses_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_COILS_H
