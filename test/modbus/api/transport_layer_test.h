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
  MOCK_METHOD(void, write, (const uint8_t* buffer, std::size_t length), (override));
  MOCK_METHOD(uint8_t*, read, (std::size_t length), (override));
  MOCK_METHOD(uint8_t*, glance, (std::size_t length), (override));
};

} // modbus_test
} // asps_test

#endif // TEST_MODBUS_API_TRANSPORT_LAYER_TEST_H
