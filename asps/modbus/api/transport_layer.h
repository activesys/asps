// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Transport Layer.

#ifndef ASPS_MODBUS_API_TRANSPORT_LAYER_H
#define ASPS_MODBUS_API_TRANSPORT_LAYER_H

#include <cstdint>

namespace asps {
namespace modbus {

/*
 * Modbus transport layer interface.
 * The user must implement this interface to provide a specific
 * transport-layer implementation for the Modbus stack, 
 * such as using Boost ASio or Libuv.
 */
class transport_layer
{
public:
  virtual void write(const uint8_t* buffer, std::size_t length) = 0;
  virtual uint8_t* read(std::size_t length) = 0;
  virtual uint8_t* glance(std::size_t length) = 0;
};

} // modbus
} // asps

#endif // ASPS_MODBUS_API_TRANSPORT_LAYER_H
