// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Transport Layer.

#ifndef ASPS_MODBUS_API_TRANSPORT_LAYER_H
#define ASPS_MODBUS_API_TRANSPORT_LAYER_H

#include <cstdint>
#include <string>
#include <functional>

namespace asps {
namespace modbus {

/*
 * Modbus transport layer interface.
 * The user must implement this interface to provide a specific
 * transport-layer implementation for the Modbus stack, 
 * such as using Boost asio or Libuv.
 */
// Client transport event
class client_transport_event
{
public:
  client_transport_event() {}
  virtual ~client_transport_event() {}

public:
  virtual void on_connect(const std::string& host, uint16_t port) = 0;
  virtual void on_error(const std::string& msg) = 0;
  virtual void on_glance(const uint8_t* buffer) = 0;
  virtual void on_read(const uint8_t* buffer) = 0;
  virtual void on_eof() = 0;
};

// Client Transport layer
class client_transport_layer
{
public:
  client_transport_layer(client_transport_event& event)
    : event_(event)
  {}

public:
  virtual void connect() = 0;
  virtual void write(const uint8_t* buffer, std::size_t length) = 0;
  virtual void read(std::size_t length) = 0;
  virtual void glance(std::size_t length) = 0;
  virtual void close() = 0;
  virtual void run() = 0;

protected:
  client_transport_event& event_;
};

// Server transport event
class server_transport_event
{
public:
  server_transport_event() {}
  virtual ~server_transport_event() {}

public:
  virtual void on_accept(const std::string& host, uint16_t port) = 0;
  virtual void on_error(const std::string& host, uint16_t port, const std::string& msg) = 0;
  virtual void on_glance(const std::string& host, uint16_t port, const uint8_t* buffer) = 0;
  virtual void on_read(const std::string& host, uint16_t port, const uint8_t* buffer) = 0;
  virtual void on_eof(const std::string& host, uint16_t port) = 0;
};

// Server Transport layer
class server_transport_layer
{
public:
  server_transport_layer(server_transport_event& event)
    : event_(event)
  {}

public:
  virtual void listen() = 0;
  virtual void write(const std::string& host, uint16_t port, const uint8_t* buffer, std::size_t length) = 0;
  virtual void read(const std::string& host, uint16_t port, std::size_t length) = 0;
  virtual void glance(const std::string& host, uint16_t port, std::size_t length) = 0;
  virtual void close(const std::string& host, uint16_t port) = 0;
  virtual void run() = 0;

protected:
  server_transport_event& event_;
};

} // modbus
} // asps

#endif // ASPS_MODBUS_API_TRANSPORT_LAYER_H
