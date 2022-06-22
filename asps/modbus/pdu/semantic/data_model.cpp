// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus data model.

#include <asps/modbus/pdu/semantic/data_model.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Discrete Inputs
bool operator==(const discrete_input& first, const discrete_input& second)
{
  return first.address == second.address &&
         first.bit == second.bit;
}

bool operator==(const discrete_inputs& first, const discrete_inputs& second)
{
  if (first.size() != second.size()) {
    return false;
  }

  for (std::size_t i = 0; i < first.size(); ++i) {
    if (!(first[i] == second[i])) {
      return false;
    }
  }

  return true;
}

void discrete_inputs::fill_address(uint16_t address)
{
  for (std::size_t i = 0; i < size(); ++i) {
    at(i).address = address + i;
  }
}

void discrete_inputs::remove_fill_data(uint16_t quantity)
{
  if (quantity < size()) {
    resize(quantity);
  }
}

// Coils
bool operator==(const coil& first, const coil& second)
{
  return first.address == second.address &&
         first.bit == second.bit;
}

bool operator==(const coils& first, const coils& second)
{
  if (first.size() != second.size()) {
    return false;
  }

  for (std::size_t i = 0; i < first.size(); ++i) {
    if (!(first[i] == second[i])) {
      return false;
    }
  }

  return true;
}

void coils::fill_address(uint16_t address)
{
  for (std::size_t i = 0; i < size(); ++i) {
    at(i).address = address + i;
  }
}

void coils::remove_fill_data(uint16_t quantity)
{
  if (quantity < size()) {
    resize(quantity);
  }
}

} // pdu
} // modbus
} // asps
