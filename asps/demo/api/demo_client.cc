// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client.

#include <boost/asio.hpp>
#include <asps/demo/api/demo_client.h>

namespace asps {
namespace demo {

using namespace boost::asio;

bool demo_client::connect()
{
  boost::system::error_code ec;
  socket_.connect(endpoint_, ec);
  is_connected_ = !ec;

  if (is_connected()) {
    session_ = make_session_service();
    session_->register_observer(this);
  }

  return is_connected();
}

bool demo_client::is_connected()
{
  return is_connected_;
}

void demo_client::close()
{
  socket_.close();
  is_connected_ = false;
  session_.reset();
}

bool demo_client::send(const data_group_type& group)
{
  if (is_connected()) {
    return session_->send(group);
  } else {
    return false;
  }
}

bool demo_client::receive(const uint8_t* buffer)
{
  return session_->receive(buffer);
}

void demo_client::update_send(const buffer_type& buf)
{
    boost::system::error_code ec;
    socket_.write_some(buffer(buf), ec);
}

void demo_client::update_event()
{
  close();
}

} // demo
} // asps
