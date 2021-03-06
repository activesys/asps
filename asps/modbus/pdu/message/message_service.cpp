// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Message Service.

#include <asps/modbus/pdu/message/message_service.hpp>
#include <asps/modbus/pdu/message/client_message.hpp>

namespace asps {
namespace modbus {
namespace pdu {

message_serialization_service::pointer_type
make_client_read_coils_request(uint16_t starting_address,
                               uint16_t quantity_of_coils)
{
  return std::make_shared<client_read_coils_request>(starting_address,
                                                     quantity_of_coils);
}

message_unserialization_service::pointer_type
make_client_read_coils_response()
{
  return std::make_shared<client_exception>(
          std::make_shared<client_read_coils_response>());
}

message_serialization_service::pointer_type
make_client_read_discrete_inputs_request(uint16_t starting_address,
                                         uint16_t quantity_of_inputs)
{
  return std::make_shared<client_read_discrete_inputs_request>(starting_address,
                                                               quantity_of_inputs);
}

message_unserialization_service::pointer_type
make_client_read_discrete_inputs_response()
{
  return std::make_shared<client_exception>(
          std::make_shared<client_read_discrete_inputs_response>());
}

message_serialization_service::pointer_type
make_client_read_holding_registers_request(uint16_t starting_address,
                                           uint16_t quantity_of_registers)
{
  return std::make_shared<client_read_holding_registers_request>(starting_address,
                                                                 quantity_of_registers);
}

message_unserialization_service::pointer_type
make_client_read_holding_registers_response()
{
  return std::make_shared<client_exception>(
          std::make_shared<client_read_holding_registers_response>());
}

message_serialization_service::pointer_type
make_client_read_input_registers_request(uint16_t statring_address,
                                         uint16_t quantity_of_registers)
{
  return std::make_shared<client_read_input_registers_request>(statring_address,
                                                               quantity_of_registers);
}

message_unserialization_service::pointer_type
make_client_read_input_registers_response()
{
  return std::make_shared<client_exception>(
          std::make_shared<client_read_input_registers_response>());
}

} // pdu
} // modbus
} // asps
