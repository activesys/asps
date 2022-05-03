// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Session.

#include <asps/demo/session/server_session.h>

namespace asps {
namespace demo {

// Server Session
server_session_service::pointer_type
make_server_session_service()
{
  return std::make_shared<server_session>();
}

server_session::server_session()
  : data_sequence_(make_server_data_sequence())
{
  data_sequence_->register_data_observer(this);
}

void server_session::receive(buffer_type& buffer)
{
  while (data_sequence_->response(buffer));
}

void server_session::update_data(const data_group_type& datas)
{
  notify_data(datas);
}

} // demo
} // asps
