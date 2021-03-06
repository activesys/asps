// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#include <arpa/inet.h>
#include <asps/modbus/adu/session/client_session.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Client Session
void client_session::receive_response(const buffer_type& adu)
{
  uint16_t tid = get_tid(adu);
  if (seqs_.count(tid)) {
    seqs_[tid]->receive_response(adu);
  }
}

void client_session::send_request(const buffer_type& pdu)
{
  client_sequence::pointer_type seq =
    std::make_shared<client_sequence>(tid_, 1, pdu);
  seq->register_event_observer(this);
  seqs_[tid_++] = seq;
  notify_send(seq->send_request());
}

void client_session::update_pdu(const buffer_type& pdu)
{
  notify_pdu(pdu);
}

uint16_t client_session::get_tid(const buffer_type& adu)
{
  const uint8_t* pos = adu.data();
  return ntohs(*reinterpret_cast<const uint16_t*>(pos));
}

} // adu
} // modbus
} // asps
