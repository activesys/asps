// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus transport layer mock.

#ifndef TEST_MODBUS_API_TRANSPORT_LAYER_TEST_H
#define TEST_MODBUS_API_TRANSPORT_LAYER_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

class client_transport_layer_mock : public client_transport_layer
{
public:
  client_transport_layer_mock(client_transport_event& event)
    : client_transport_layer(event)
  {}

public:
  MOCK_METHOD(void, connect, (), (override));
  MOCK_METHOD(void, write, (const uint8_t* buffer, std::size_t length), (override));
  MOCK_METHOD(void, read, (std::size_t length), (override));
  MOCK_METHOD(void, glance, (std::size_t length), (override));
  MOCK_METHOD(void, close, (), (override));
  MOCK_METHOD(void, run, (), (override));
};

class server_transport_layer_mock : public server_transport_layer
{
public:
  server_transport_layer_mock(server_transport_event& event)
    : server_transport_layer(event)
  {}

public:
  MOCK_METHOD(void, listen, (), (override));
  MOCK_METHOD(void, write, (const std::string& host, uint16_t port, const uint8_t* buffer, std::size_t length), (override));
  MOCK_METHOD(void, read, (const std::string& host, uint16_t port, std::size_t length), (override));
  MOCK_METHOD(void, glance, (const std::string& host, uint16_t port, std::size_t length), (override));
  MOCK_METHOD(void, close, (const std::string& host, uint16_t port), (override));
  MOCK_METHOD(void, run, (), (override));
};

} // modbus_test
} // asps_test

#endif // TEST_MODBUS_API_TRANSPORT_LAYER_TEST_H
