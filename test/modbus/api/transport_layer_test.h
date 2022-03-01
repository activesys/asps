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

class transport_layer_mock : public transport_layer
{
public:
  MOCK_METHOD(void, connect, (connect_handler, error_handler), (override));
  MOCK_METHOD(void, write, (const uint8_t*, std::size_t, eof_handler, error_handler), (override));
  MOCK_METHOD(void, read, (std::size_t, read_handler, eof_handler, error_handler), (override));
  MOCK_METHOD(void, glance, (std::size_t, glance_handler, eof_handler, error_handler), (override));
};

} // modbus_test
} // asps_test

#endif // TEST_MODBUS_API_TRANSPORT_LAYER_TEST_H
