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
}

void demo_client::compress_same_type(bool flag)
{
  session_.compress_same_type(flag);
}

void demo_client::compress_same_timestamp(bool flag)
{
  session_.compress_same_timestamp(flag);
}

} // demo
} // asps
