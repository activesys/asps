// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Server.

#ifndef ASPS_MODBUS_API_SERVER_H
#define ASPS_MODBUS_API_SERVER_H

#include <cstdint>
#include <boost/asio.hpp>

#include <asps/modbus/api/event.h>
#include <asps/modbus/session/session.h>

namespace asps {
namespace modbus {

using boost::asio::ip::tcp;

// Modbus Server
class server
{
public:
  explicit server(uint16_t port = 502)
    : context_(),
      acceptor_(context_, tcp::endpoint(tcp::v4(), port)),
      event_(nullptr)
  {}
  ~server() {delete event_;}

public:
  void register_event(server_event* event);
  void async_listen();
  void run();

private:
  boost::asio::io_context context_;
  tcp::acceptor acceptor_;
  server_event* event_;
  server_session_set_type sessions_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_SERVER_H
