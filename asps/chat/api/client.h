// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Client.

#ifndef ASPS_CHAT_API_CLIENT_H
#define ASPS_CHAT_API_CLIENT_H

#include <string>
#include <thread>
#include <boost/asio.hpp>

#include <asps/chat/api/event.h>
#include <asps/chat/session/session.h>

namespace asps {
namespace chat {

using boost::asio::ip::tcp;

// Chat client
class client
{
public:
  client(const std::string& host, uint16_t port)
    : context_(),
      socket_(context_),
      event_(nullptr)
  {
    tcp::resolver resolver(context_);
    endpoint_ = resolver.resolve(host, std::to_string(port));
  }
  ~client()
  {
    delete event_;
  }

public:
  // Async interface
  void register_event(client_event* event);
  void async_connect();
  uint16_t async_send(const std::string& msg);
  void run();
  void close();

private:
  boost::asio::io_context context_;
  tcp::socket socket_;
  tcp::resolver::results_type endpoint_;
  client_session_ptr session_;
  client_event* event_;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_API_CLIENT_H
