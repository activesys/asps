// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU sequence.

#ifndef ASPS_MODBUS_ADU_SEQUENCE_TCP_ADU_SEQUENCE_H
#define ASPS_MODBUS_ADU_SEQUENCE_TCP_ADU_SEQUENCE_H

#include <cstdint>
#include <memory>
#include <asps/modbus/api/event.h>
#include <asps/modbus/adu/message/tcp_adu.h>
#include <asps/modbus/semantic/coils.h>
#include <asps/modbus/semantic/constant.h>
#include <asps/modbus/pdu/sequence/pdu_sequence.h>

namespace asps {
namespace modbus {

// Modbus TCP ADU Client Sequence
class tcp_adu_client_sequence
{
public:
  typedef std::shared_ptr<tcp_adu_client_sequence> pointer_type;

public:
  tcp_adu_client_sequence(
    uint16_t transaction_identifier,
    uint8_t unit_identifier)
    : transaction_identifier_(transaction_identifier),
      unit_identifier_(unit_identifier)
  {}

public:
  tcp_adu::pointer_type get_request(
    const coils::pointer_type cs, function_codes code);
  void set_response(tcp_adu::pointer_type adu);

private:
  uint16_t transaction_identifier_;
  uint8_t unit_identifier_;
  pdu_client_sequence_ptr pdu_sequence_;
};

// Modbus TCP ADU Server sequence
class tcp_adu_server_sequence
{
public:
  typedef std::shared_ptr<tcp_adu_server_sequence> pointer_type;

public:
  tcp_adu_server_sequence(server_event* event)
    : event_(event)
  {}

public:
  tcp_adu::pointer_type set_request(tcp_adu::pointer_type adu);

private:
  server_event* event_;
  pdu_server_sequence::pointer_type pdu_sequence_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_ADU_SEQUENCE_TCP_ADU_SEQUENCE_H
