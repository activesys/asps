// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU State.

#include <asps/modbus/pdu/sequence/state.hpp>
#include <asps/modbus/pdu/sequence/sequence_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Idle State
state* idle_state::instance_ = nullptr;

const buffer_type& idle_state::request(active_sequence_service* seq)
{
  seq->change_state(sent_state::instance());
  return seq->serialize();
}

bool idle_state::response(active_sequence_service* seq,
                          const buffer_type& buffer)
{
  return seq->unserialize(buffer);
}

// Sent State
state* sent_state::instance_ = nullptr;

const buffer_type& sent_state::request(active_sequence_service* seq)
{
  return empty_buffer_;
}

bool sent_state::response(active_sequence_service* seq,
                          const buffer_type& buffer)
{
  seq->change_state(idle_state::instance());
  bool ret = seq->unserialize(buffer);
  seq->notify();

  return ret;
}

} // pdu
} // demo
} // asps

