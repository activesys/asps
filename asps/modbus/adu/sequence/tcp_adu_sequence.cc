// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU sequence.

#include <asps/modbus/adu/sequence/tcp_adu_sequence.h>

using namespace asps::modbus;

// Modbus TCP ADU Client Sequence
tcp_adu tcp_adu_client_sequence::get_request(const coils::ptr_type cs)
{
  pdu_sequence_ =
    std::make_shared<read_coils_pdu_client_sequence>(cs, event_);
  pdu_ptr pdu = pdu_sequence_->get_request(true);

  return tcp_adu(transaction_identifier_, unit_identifier_, pdu);
}

void tcp_adu_client_sequence::set_response(tcp_adu& adu)
{
  if (event_) {
    if (adu.transaction_identifier() == transaction_identifier_ &&
        adu.unit_identifier() == unit_identifier_) {
        pdu_sequence_->set_response(adu.pdu());
    } else {
        event_->on_error("Invalid ADU");
    }
  }
}

// Modbus TCP ADU Server Sequence
tcp_adu tcp_adu_server_sequence::set_request(tcp_adu& adu)
{
  uint8_t function_code = adu.pdu()->function_code();
  switch (function_code) {
    case pdu::read_coils:
      pdu_sequence_ = std::make_shared<read_coils_pdu_server_sequence>(event_);
    break;

    default:
      pdu_sequence_ = std::make_shared<invalid_pdu_server_sequence>(event_);
    break;
  }

  return tcp_adu(
          adu.transaction_identifier(),
          adu.unit_identifier(),
          pdu_sequence_->set_request(adu.pdu()));
}
