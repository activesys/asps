// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Server.

#ifndef ASPS_CHAT_SERVER_H
#define ASPS_CHAT_SERVER_H

#include <cstdint>
#include <boost/asio.hpp>

#include <asps/chat/event.h>
#include <asps/chat/session/session.h>

namespace asps {
namespace chat {

using boost::asio::ip::tcp;

// Chat Server
class server
{
public:
  server(uint16_t port)
    : context_(),
      acceptor_(context_, tcp::endpoint(tcp::v4(), port)),
      event_(nullptr)
  {}

public:
  void register_event(server_event* event);
  void async_listen();
  void run();

private:
  boost::asio::io_context context_;
  tcp::acceptor acceptor_;
  server_session_set_type sessions_;
  server_event* event_;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_SERVER_H
