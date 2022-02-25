// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU.

#ifndef ASPS_MODBUS_ADU_MESSAGE_TCP_ADU_H
#define ASPS_MODBUS_ADU_MESSAGE_TCP_ADU_H

#include <cstdint>
#include <vector>
#include <asps/modbus/pdu/message/pdu.h>

namespace asps {
namespace modbus {

// Modbus TCP ADU
class tcp_adu
{
  enum field_length {
    transaction_identifier_field_length = 2,
    protocol_identifier_field_length = 2,
    length_field_length = 2,
    unit_identifier_field_length = 1
  };
  enum default_value {
    default_protocol_identifier = 0x00
  };

public:
  typedef std::shared_ptr<tcp_adu> pointer_type;

public:
  tcp_adu(
    uint16_t transaction_identifier,
    uint8_t unit_identifier,
    mb_pdu::pointer_type pdu)
    : transaction_identifier_(transaction_identifier),
      unit_identifier_(unit_identifier),
      pdu_(pdu)
  {}

public:
  uint16_t transaction_identifier() {return transaction_identifier_;}
  uint8_t unit_identifier() {return unit_identifier_;}
  mb_pdu::pointer_type pdu() {return pdu_;}

  uint8_t* serialize();
  std::size_t serialized_size();

public:
  static std::size_t mbap_header_size();
  static uint16_t pdu_size(const uint8_t* buffer);
  static tcp_adu::pointer_type unserialize(const uint8_t* buffer, bool is_request);

private:
  uint16_t transaction_identifier_;
  uint8_t unit_identifier_;
  std::vector<uint8_t> buffer_;
  mb_pdu::pointer_type pdu_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_ADU_MESSAGE_TCP_ADU_H
