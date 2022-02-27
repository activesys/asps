// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Implement transport Layer with Boost ASio.

#ifndef DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
#define DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H

#include <boost/asio.hpp>
#include <asps/modbus/api/transport_layer.h>

namespace asps_demos {
namespace modbus_demos {

using namespace asps::modbus;

class boost_asio_transport_layer : public transport_layer
{
public:
  void write(const uint8_t* msg, std::size_t length) override {}
  uint8_t* read(std::size_t length) override {return nullptr;}
  uint8_t* glance(std::size_t length) override {return nullptr;}
};

} // modbus_demos
} // asps_demos

#endif // DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
