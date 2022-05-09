// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Sequence.

#include <asps/demo/sequence/server_sequence.h>

namespace asps {
namespace demo {

// Server Data Sequence
server_data_sequence::server_data_sequence()
  : message_(make_server_data_message())
{}

bool server_data_sequence::receive_data(buffer_type& buffer)
{
  if (message_->unserialize(buffer)) {
    notify_data(message_->datas());
    return true;
  } else {
    return false;
  }
}

// Server Positive Keepalive Sequence
server_positive_keepalive_sequence::server_positive_keepalive_sequence()
{
  keepalive_ = make_server_positive_keepalive();
  ack_ = make_server_positive_keepalive_ack();
}

// Server Negative Keepalive Sequence
server_negative_keepalive_sequence::server_negative_keepalive_sequence()
{
  keepalive_ = make_server_negative_keepalive();
  ack_ = make_server_negative_keepalive_ack();
}

} // demo
} // asps
