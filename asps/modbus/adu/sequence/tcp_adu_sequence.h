// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU sequence.

#ifndef BOOST_MODBUS_ADU_SEQUENCE_TCPADUSEQUENCE_H
#define BOOST_MODBUS_ADU_SEQUENCE_TCPADUSEQUENCE_H

#include <cstdint>
#include <memory>
#include <asps/modbus/api/event.h>
#include <asps/modbus/adu/message/tcp_adu.h>
#include <asps/modbus/semantic/coils.h>
#include <asps/modbus/pdu/sequence/pdu_sequence.h>

namespace asps {
namespace modbus {

// Modbus TCP ADU Client Sequence
class tcp_adu_client_sequence
{
public:
  tcp_adu_client_sequence(
    uint16_t transaction_identifier,
    uint8_t unit_identifier,
    client_event* event)
    : transaction_identifier_(transaction_identifier),
      unit_identifier_(unit_identifier),
      event_(event)
  {}

public:
  tcp_adu get_request(const coils::ptr_type cs);
  void set_response(tcp_adu& adu);

private:
  uint16_t transaction_identifier_;
  uint8_t unit_identifier_;
  client_event* event_;
  pdu_client_sequence_ptr pdu_sequence_;
};

typedef std::shared_ptr<tcp_adu_client_sequence> tcp_adu_client_sequence_ptr;

} // namespace modbus
} // namespace asps

#endif // BOOST_MODBUS_ADU_SEQUENCE_TCPADUSEQUENCE_H