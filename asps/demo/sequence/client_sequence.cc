// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Sequence.

#include <asps/demo/sequence/client_sequence.h>

namespace asps {
namespace demo {

// Client Data Sequence
const buffer_type& client_data_sequence::send_data(const data_group_type& group)
{
  message_ = make_client_data_message(group);
  return message_->serialize();
}

// Client Positive Keepalive Sequence
client_positive_keepalive_sequence::client_positive_keepalive_sequence()
{
  keepalive_ = make_client_positive_keepalive();
  ack_ = make_client_positive_keepalive_ack();
}

// Client Negative Keepalive Sequence
client_negative_keepalive_sequence::client_negative_keepalive_sequence()
{
  keepalive_ = make_client_negative_keepalive();
  ack_ = make_client_negative_keepalive_ack();
}

} // demo
} // asps
