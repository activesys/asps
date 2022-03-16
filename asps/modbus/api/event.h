// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Event.

#ifndef ASPS_MODBUS_API_EVENT_H
#define ASPS_MODBUS_API_EVENT_H

#include <cstdint>
#include <string>

#include <asps/modbus/semantic/constant.h>
#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

// Modbus Client Event
class client;
class client_event
{
public:
  client_event(client& c)
    : modbus_client(c)
  {}
  virtual ~client_event() {}

public:
  virtual void on_connect(const std::string& address, uint16_t port) = 0;
  virtual void on_error(const std::string& error_message) = 0;

public:
  virtual void on_read_coils(const coils::pointer_type cs,
                             exception_code code) = 0;
  virtual void on_write_single_coil(const coils::pointer_type cs,
                                    exception_code code) = 0;
  virtual void on_write_multiple_coils(const coils::pointer_type cs,
                                       exception_code code) = 0;

protected:
  client& modbus_client;
};

// Server event
class server;
class server_event
{
public:
  explicit server_event(server& s)
    : modbus_server(s)
  {}
  virtual ~server_event() {}

public:
  virtual void on_accept(const std::string& address, uint16_t port) = 0;
  virtual void on_accept(const std::string& error_message) = 0;

  virtual void on_error(const std::string& error_message) = 0;

public:
  virtual coils::pointer_type on_read_coils(const coils::pointer_type cs) = 0;
  virtual coils::pointer_type on_write_single_coil(const coils::pointer_type cs) = 0;
  virtual coils::pointer_type on_write_multiple_coils(const coils::pointer_type cs) = 0;

protected:
  server& modbus_server;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_API_EVENT_H
