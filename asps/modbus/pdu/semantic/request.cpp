// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Request.

#include <asps/modbus/pdu/semantic/request.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Read Coils Request
request::pointer_type make_read_coils_request(uint16_t address,
                                              uint16_t quantity)
{
  return std::make_shared<read_coils_request>(address, quantity);
}

bool read_coils_request::valid()
{
  return quantity_of_coils_ > 0;
}

request::pointer_type read_coils_request::split()
{
  uint16_t address = starting_address_;
  uint16_t quantity = quantity_of_coils_ < max_quantity_of_coils ?
                      quantity_of_coils_ : max_quantity_of_coils;

  starting_address_ += quantity;
  quantity_of_coils_ -= quantity;

  return make_read_coils_request(address, quantity);
}

// Read Discrete Inputs Request
request::pointer_type make_read_discrete_inputs_request(uint16_t address,
                                                        uint16_t quantity)
{
  return std::make_shared<read_discrete_inputs_request>(address, quantity);
}

bool read_discrete_inputs_request::valid()
{
  return quantity_of_inputs_ > 0;
}

request::pointer_type read_discrete_inputs_request::split()
{
  uint16_t address = starting_address_;
  uint16_t quantity = quantity_of_inputs_ < max_quantity_of_inputs ?
                      quantity_of_inputs_ : max_quantity_of_inputs;

  starting_address_ += quantity;
  quantity_of_inputs_ -= quantity;

  return make_read_discrete_inputs_request(address, quantity);
}

} // pdu
} // modbus
} // asps
