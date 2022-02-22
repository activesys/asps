// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU sequence.

#include <asps/modbus/semantic/constant.h>
#include <asps/modbus/adu/sequence/tcp_adu_sequence.h>

using namespace asps::modbus;

// Modbus TCP ADU Client Sequence
tcp_adu::pointer_type
tcp_adu_client_sequence::get_request(
  const coils::ptr_type cs, function_codes code)
{
  switch (code) {
  case read_coils:
    pdu_sequence_ =
      std::make_shared<read_coils_pdu_client_sequence>(cs, event_);
    break;

  case write_single_coil:
    pdu_sequence_ =
      std::make_shared<write_single_coil_pdu_client_sequence>(cs, event_);
    break;

  case write_multiple_coils:
    pdu_sequence_ =
      std::make_shared<write_multiple_coils_pdu_client_sequence>(cs, event_);
    break;

  default:
    pdu_sequence_ =
      std::make_shared<invalid_pdu_client_sequence>(event_);
    break;
  }

  return std::make_shared<tcp_adu>(
          transaction_identifier_,
          unit_identifier_,
          pdu_sequence_->get_request());
}

void tcp_adu_client_sequence::set_response(tcp_adu::pointer_type adu)
{
  if (event_) {
    if (adu->transaction_identifier() == transaction_identifier_ &&
        adu->unit_identifier() == unit_identifier_) {
      pdu_sequence_->set_response(adu->pdu());
    } else {
      event_->on_error("Invalid ADU");
    }
  }
}

// Modbus TCP ADU Server Sequence
tcp_adu::pointer_type
tcp_adu_server_sequence::set_request(tcp_adu::pointer_type adu)
{
  uint8_t function_code = adu->pdu()->function_code();
  switch (function_code) {
  case read_coils:
    pdu_sequence_ =
      std::make_shared<read_coils_pdu_server_sequence>(event_);
    break;

  case write_single_coil:
    pdu_sequence_ =
      std::make_shared<write_single_coil_pdu_server_sequence>(event_);
    break;

  case write_multiple_coils:
    pdu_sequence_ =
      std::make_shared<write_multiple_coils_pdu_server_sequence>(event_);
    break;

  default:
    pdu_sequence_ =
      std::make_shared<invalid_pdu_server_sequence>(event_);
    break;
  }

  return std::make_shared<tcp_adu>(
          adu->transaction_identifier(),
          adu->unit_identifier(),
          pdu_sequence_->set_request(adu->pdu()));
}
