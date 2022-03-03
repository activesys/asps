// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Server.

#ifndef ASPS_MODBUS_API_SERVER_H
#define ASPS_MODBUS_API_SERVER_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <asps/modbus/api/transport_layer.h>
#include <asps/modbus/api/event.h>
#include <asps/modbus/session/session.h>

namespace asps {
namespace modbus {

// Modbus Server
class server
{
  /*
  typedef std::unordered_map<std::pair<std::string&, uint16_t>,
                             server_session::pointer_type> sessions_type;
                             */

public:
  explicit server(transport_layer& layer)
    : transport_layer_(layer)
  {}

public:
  void event(server_event* e);
  void listen();
  void run();

private:
  void on_accept(const std::string& host, uint16_t port);
  void on_error(const std::string& error_message);

private:
  transport_layer& transport_layer_;
  //sessions_type sessions_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_SERVER_H
