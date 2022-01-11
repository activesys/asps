// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Client.

#ifndef ASPS_CHAT_CLIENT_H
#define ASPS_CHAT_CLIENT_H

#include <functional>
#include <string>
#include <thread>
#include <boost/asio.hpp>

#include <asps/chat/notify.h>
#include <asps/chat/session/session.h>

namespace asps {
namespace chat {

using boost::asio::ip::tcp;

// Chat client
class client
{
public:
  client(const std::string& host, uint16_t port, client_notify notify)
    : context_(),
      socket_(context_),
      notify_(notify)
  {
    tcp::resolver resolver(context_);
    endpoints_ = resolver.resolve(host, std::to_string(port));
  }
  ~client() {}

public:
  bool connect();
  void close();

  uint16_t send(const std::string& msg);

private:
  boost::asio::io_context context_;
  tcp::socket socket_;
  tcp::resolver::results_type endpoints_;
  std::thread t_;
  client_session_ptr session_;
  client_notify notify_;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_CLIENT_H
