// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client.

#ifndef ASPS_DEMO_API_DEMO_CLIENT_H
#define ASPS_DEMO_API_DEMO_CLIENT_H

#include <cstdint>
#include <string>
#include <boost/asio.hpp>

#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/session/demo_session.h>

namespace asps {
namespace demo {

using namespace boost::asio;

class demo_client
{
public:
  demo_client(const std::string& host, uint16_t port)
    : socket_(context_),
      endpoint_(ip::address::from_string(host), port),
      is_connected_(false),
      session_(false, false)
  {}

public:
  bool connect();
  bool is_connected();
  void close();
  void compress_same_type(bool flag);
  void compress_same_timestamp(bool flag);

  template <typename IT>
  bool send(IT first, IT second)
  {
    const std::vector<uint8_t>& buf = session_.send(first, second);
    boost::system::error_code ec;
  
    socket_.write_some(buffer(buf), ec);
    return !ec;
  }

private:
  io_context context_;
  ip::tcp::socket socket_;
  ip::tcp::endpoint endpoint_;
  bool is_connected_;
  demo_session session_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_CLIENT_H
