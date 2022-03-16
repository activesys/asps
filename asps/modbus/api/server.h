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
#include <vector>
#include <string>
#include <utility>

#include <asps/modbus/api/transport_layer.h>
#include <asps/modbus/api/event.h>
#include <asps/modbus/session/session.h>

// Address
typedef std::pair<std::string, uint16_t> address;
namespace std {
  template<>
  struct hash<address>
  {
    public:
      size_t operator()(const address& addr) const
      {
        return hash<string>()(addr.first) ^ hash<uint16_t>()(addr.second);
      }
  };

  template<>
  struct equal_to<address>
  {
    public:
      bool operator()(const address& p1, const address& p2) const
      {
        return p1.first == p2.first && p1.second == p2.second;
      }
  };
}

namespace asps {
namespace modbus {

// Modbus Server
class server : public server_transport_event
{
  typedef std::unordered_map<address, server_session::pointer_type> sessions_type;

public:
  void transport_layer(server_transport_layer* layer);
  void event(server_event* e);
  void listen();
  void run();

private:
  void on_accept(const std::string& host, uint16_t port) override;
  void on_error(const std::string& host, uint16_t port, const std::string& msg) override;
  void on_glance(const std::string& host, uint16_t port, const uint8_t* buffer) override;
  void on_read(const std::string& host, uint16_t port, const uint8_t* buffer) override;
  void on_eof(const std::string& host, uint16_t port) override;

private:
  server_transport_layer* transport_layer_;
  sessions_type sessions_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_SERVER_H
