// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client.

#ifndef ASPS_MODBUS_API_CLIENT_H
#define ASPS_MODBUS_API_CLIENT_H

#include <cstdint>
#include <string>
#include <boost/asio.hpp>

#include <asps/modbus/api/event.h>
#include <asps/modbus/session/client_session.h>
#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

using boost::asio::ip::tcp;

// Modbus Client
class client
{
public:
  client(const std::string& host, uint16_t port = 502)
    : context_(),
      socket_(context_),
      event_(nullptr)
  {
    tcp::resolver resolver(context_);
    endpoint_ = resolver.resolve(host, std::to_string(port));
  }
  ~client() {delete event_;}

public:
  // Async interface
  void register_event(client_event* event);
  void async_connect();
  void run();
  void close();

  void read_coils(uint8_t unit_identifier, const coils& cs);

private:
  boost::asio::io_context context_;
  tcp::socket socket_;
  tcp::resolver::results_type endpoint_;
  client_event* event_;
  client_session_ptr session_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_CLIENT_H
