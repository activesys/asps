// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#include <memory>
#include <asps/modbus/pdu/sequence/pdu_sequence.h>

using namespace asps::modbus;

// Modbus read coils request pdu client sequence
pdu_ptr read_coils_request_pdu_client_sequence::get_request(bool is_read)
{
  if (is_read) {
    return std::make_shared<read_coils_request>(
             coils_->starting_address(), coils_->count());
  } else if (coils_->count() == 1) {
    return std::make_shared<write_single_coil_request>(
             coils_->starting_address(), coils_->at(0));
  } else {
    pdu::coils_type coils;
    for (uint8_t i = 0; i < coils_->count(); ++i) {
      coils.push_back(coils_->at(coils_->starting_address() + i));
    }
    return std::make_shared<write_multiple_coils_request>(
             coils_->starting_address(), coils_->count(), coils);
  }
}

void read_coils_request_pdu_client_sequence::set_response(pdu_ptr pdu)
{
  if (event_) {
    if (read_coils_response* p = dynamic_cast<read_coils_response*>(pdu.get())) {
      for (uint16_t i = 0; i < coils_->count(); ++i) {
        coils_->at(coils_->starting_address() + i) = p->status().at(i);
      }
      event_->on_read_coils(coils_, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event_->on_read_coils(coils_, e->code());
    }
  }
}
