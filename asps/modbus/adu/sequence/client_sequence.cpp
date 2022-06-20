// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Sequence.

#include <asps/modbus/adu/sequence/client_sequence.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Client Sequence
const buffer_type& client_sequence::serialize()
{
  return request_.serialize();
}

bool client_sequence::unserialize(const buffer_type& buffer)
{
  return response_.unserialize(buffer);
}

void client_sequence::notify()
{
  notify_pdu(response_.pdu());
}

const buffer_type& client_sequence::send_request()
{
  return state_->request(this);
}

void client_sequence::change_state(state* s)
{
  state_ = s;
}

bool client_sequence::receive_response(const buffer_type& adu)
{
  return state_->response(this, adu);
}

} // adu
} // demo
} // asps
