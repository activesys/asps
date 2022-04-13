// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo State.

#include <asps/demo/sequence/state.h>
#include <asps/demo/sequence/sequence.h>

namespace asps {
namespace demo {

state* none_state::instance_ = nullptr;

const buffer_type& none_state::request(positive_keepalive_sequence* seq)
{
  seq->change_state(keepalive_sent_state::instance());
  seq->t2_start();
  return seq->serialize();
}

bool none_state::response(positive_keepalive_sequence* seq,
                          const uint8_t* buffer)
{
  return false;
}

void none_state::timeout(positive_keepalive_sequence* seq)
{
}


state* keepalive_sent_state::instance_ = nullptr;

const buffer_type& keepalive_sent_state::request(positive_keepalive_sequence* seq)
{
  return empty_buffer_;
}

bool keepalive_sent_state::response(positive_keepalive_sequence* seq,
                                    const uint8_t* buffer)
{
  seq->change_state(none_state::instance());
  seq->t2_stop();
  return seq->unserialize(buffer);
}

void keepalive_sent_state::timeout(positive_keepalive_sequence* seq)
{
  seq->change_state(none_state::instance());
  seq->t2_start();
  seq->notify_event();
}

} // demo
} // asps

