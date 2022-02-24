// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Coils and Discrete Inputs.

#include <algorithm>
#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

bool coils::bit(uint16_t address) const
{
  if (status_.size() < count_ ||
      address < starting_address_ ||
      address >= starting_address_ + count_) {
    throw std::out_of_range("Invalid address");
  }

  return status_[address - starting_address_];
}

void coils::bit(uint16_t address, bool s)
{
  if (status_.size() < count_ ||
      address < starting_address_ ||
      address >= starting_address_ + count_) {
    throw std::out_of_range("Invalid address");
  }

  status_[address - starting_address_] = s;
}

} // modbus
} // asps
