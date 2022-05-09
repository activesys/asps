// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo State.

#include <asps/demo/sequence/state.h>
#include <asps/demo/sequence/client_sequence.h>

namespace asps {
namespace demo {

state* none_state::instance_ = nullptr;

const buffer_type& none_state::request(active_sequence* seq)
{
  seq->change_state(sent_state::instance());
  seq->t2_start();
  return seq->serialize();
}

bool none_state::response(active_sequence* seq, buffer_type& buffer)
{
  return seq->unserialize(buffer);
}

void none_state::timeout(active_sequence* seq)
{
}

state* sent_state::instance_ = nullptr;

const buffer_type& sent_state::request(active_sequence* seq)
{
  return empty_buffer_;
}

bool sent_state::response(active_sequence* seq, buffer_type& buffer)
{
  seq->change_state(none_state::instance());
  seq->t2_stop();
  return seq->unserialize(buffer);
}

void sent_state::timeout(active_sequence* seq)
{
  seq->change_state(none_state::instance());
  seq->t2_start();
  seq->notify_event();
}

} // demo
} // asps

