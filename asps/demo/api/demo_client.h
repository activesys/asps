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

#include <asps/demo/api/session_service.h>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

using namespace boost::asio;

class demo_client : public observer
{
public:
  demo_client(const std::string& host, uint16_t port)
    : socket_(context_),
      endpoint_(ip::address::from_string(host), port),
      is_connected_(false)
  {}

public:
  bool connect();
  bool is_connected();
  void close();
  void same_type(bool flag);
  void key_sequence(bool flag);
  void same_timestamp(bool flag);

  bool send(const data_group_type& group)
  {
    if (is_connected()) {
      const std::vector<uint8_t>& buf = session_->serialize_datas(group);
      boost::system::error_code ec;
    
      socket_.write_some(buffer(buf), ec);
      return !ec;
    } else {
      return false;
    }
  }

  bool receive(const uint8_t* buffer)
  {
    return session_->receive(buffer);
  }

  void update_positive_keepalive()
  {
    const std::vector<uint8_t>& buf = session_->serialize_keepalive();
    boost::system::error_code ec;
  
    socket_.write_some(buffer(buf), ec);
  }

  void update_missing_positive_keepalive_ack()
  {
    close();
  }

private:
  io_context context_;
  ip::tcp::socket socket_;
  ip::tcp::endpoint endpoint_;
  bool is_connected_;
  session_service::pointer_type session_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_CLIENT_H
