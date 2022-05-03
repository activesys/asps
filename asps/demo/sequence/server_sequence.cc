// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Sequence.

#include <asps/demo/sequence/server_sequence.h>

namespace asps {
namespace demo {

// Server Data Sequence
server_data_sequence_service::pointer_type
make_server_data_sequence()
{
  return std::make_shared<server_data_sequence>();
}

server_data_sequence::server_data_sequence()
  : message_(make_message_unserialization_service())
{}

bool server_data_sequence::response(buffer_type& buffer)
{
  if (message_->unserialize(buffer)) {
    notify_data(message_->datas());
    return true;
  } else {
    return false;
  }
}

} // demo
} // asps
