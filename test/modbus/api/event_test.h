// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus event mock.

#ifndef TEST_MODBUS_API_EVENT_TEST_H
#define TEST_MODBUS_API_EVENT_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

class client_event_mock : public client_event
{
public:
  client_event_mock(client& c)
    : client_event(c)
  {}

public:
  MOCK_METHOD(void, on_connect, (const std::string& address, uint16_t port), (override));
  MOCK_METHOD(void, on_connect, (const std::string& error_message), (override));
  MOCK_METHOD(void, on_error, (const std::string& error_message), (override));
  MOCK_METHOD(void, on_read_coils, (const coils::pointer_type cs, exception_code code), (override));
  MOCK_METHOD(void, on_write_single_coil, (const coils::pointer_type cs, exception_code code), (override));
  MOCK_METHOD(void, on_write_multiple_coils, (const coils::pointer_type cs, exception_code code), (override));
};

class server_event_mock : public server_event
{
public:
  server_event_mock(server& s)
    : server_event(s)
  {}

public:
  MOCK_METHOD(void, on_accept, (const std::string& address, uint16_t port), (override));
  MOCK_METHOD(void, on_accept, (const std::string& error_message), (override));
  MOCK_METHOD(void, on_error, (const std::string& error_message), (override));
  MOCK_METHOD(coils::pointer_type, on_read_coils, (const coils::pointer_type cs), (override));
  MOCK_METHOD(coils::pointer_type, on_write_single_coil, (const coils::pointer_type cs), (override));
  MOCK_METHOD(coils::pointer_type, on_write_multiple_coils, (const coils::pointer_type cs), (override));
};

} // modbus_test
} // asps_test

#endif // TEST_MODBUS_API_EVENT_TEST_H