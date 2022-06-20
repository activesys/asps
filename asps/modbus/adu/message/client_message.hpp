// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Client Message.

#ifndef ASPS_MODBUS_ADU_MESSAGE_CLIENT_MESSAGE_HPP
#define ASPS_MODBUS_ADU_MESSAGE_CLIENT_MESSAGE_HPP

#include <asps/utility/utility.h>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::utility;

const static uint16_t modbus_protocol = 0;

enum message_field_length
{
  transaction_identifier_field_length = 2,
  protocol_identifier_field_length = 2,
  length_field_length = 2,
  unit_identifier_field_length = 1,
  mbap_field_length = 7
};

// Client ADU
class client_request_adu
{
public:
  client_request_adu(uint16_t tid, uint8_t uid, const buffer_type& pdu)
    : tid_(tid),
      pid_(modbus_protocol),
      uid_(uid),
      pdu_(pdu)
  {}

public:
  const buffer_type& serialize();

private:
  uint16_t tid_;
  uint16_t pid_;
  uint8_t uid_;
  buffer_type pdu_;
  buffer_type buffer_;
};

class client_response_adu
{
public:
  bool unserialize(const buffer_type& buffer);

public:
  const buffer_type& pdu()
  {return pdu_;}
  uint16_t tid()
  {return tid_;}
  uint16_t pid()
  {return pid_;}
  uint8_t uid()
  {return uid_;}

private:
  buffer_type pdu_;
  uint16_t tid_;
  uint16_t pid_;
  uint8_t uid_;
};

} // adu
} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_ADU_MESSAGE_CLIENT_MESSAGE_HPP
