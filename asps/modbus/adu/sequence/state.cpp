// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU State.

#include <asps/modbus/adu/sequence/state.hpp>
#include <asps/modbus/adu/sequence/sequence.hpp>

namespace asps {
namespace modbus {
namespace adu {

state* idle_state::instance_ = nullptr;

const buffer_type& idle_state::request(active_sequence* seq)
{
  seq->change_state(sent_state::instance());
  //seq->t2_start();
  return seq->serialize();
}

bool idle_state::response(active_sequence* seq, const buffer_type& buffer)
{
  return seq->unserialize(buffer);
}

/*
void idle_state::timeout(active_sequence* seq)
{
}
*/

state* sent_state::instance_ = nullptr;

const buffer_type& sent_state::request(active_sequence* seq)
{
  return empty_buffer_;
}

bool sent_state::response(active_sequence* seq, const buffer_type& buffer)
{
  seq->change_state(idle_state::instance());
  //seq->t2_stop();
  bool ret = seq->unserialize(buffer);
  seq->notify();

  return ret;
}

/*
void sent_state::timeout(active_sequence* seq)
{
  seq->change_state(idle_state::instance());
  seq->t2_start();
  seq->notify_event();
}
*/

} // pdu
} // demo
} // asps

