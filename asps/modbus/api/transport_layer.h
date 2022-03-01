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
class transport_layer
{
public:
  typedef std::function<void (const std::string&, uint16_t)> connect_handler;
  typedef std::function<void (const std::string&)> error_handler;
  typedef std::function<void ()> eof_handler;
  typedef std::function<void (const uint8_t*)> glance_handler;
  typedef std::function<void (const uint8_t*)> read_handler;

public:
  virtual void connect(connect_handler on_connect,
                       error_handler on_error) = 0;
  virtual void write(const uint8_t* buffer,
                     std::size_t length,
                     eof_handler on_eof,
                     error_handler on_error) = 0;
  virtual void read(std::size_t length,
                    read_handler on_read,
                    eof_handler on_eof,
                    error_handler on_error) = 0;
  virtual void glance(std::size_t length,
                      glance_handler on_glance,
                      eof_handler on_eof,
                      error_handler on_error) = 0;
};

} // modbus
} // asps

#endif // ASPS_MODBUS_API_TRANSPORT_LAYER_H
