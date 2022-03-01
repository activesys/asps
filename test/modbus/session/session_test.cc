// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Session.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/modbus.h>
#include <test/modbus/api/transport_layer_test.h>
#include <test/modbus/api/event_test.h>

namespace asps_test {
namespace modbus_test {

using ::testing::_;
using namespace asps::modbus;

// TEST client session
TEST(client_session_test, mbap_header_size)
{
  transport_layer_mock layer;
  client_session session(1, layer);
  EXPECT_EQ(session.mbap_header_size(), 7);
}

TEST(client_session_test, adu_size)
{
  transport_layer_mock layer;
  client_session session(1, layer);
  uint8_t buffer[7] = {0x00, 0x01, 0x00, 0x00, 0x12, 0x34, 0x01};
  EXPECT_EQ(session.adu_size(buffer), 4666);
}

TEST(client_session_test, read_coils_one)
{
  transport_layer_mock layer;
  client_session session(1, layer);
  coils cs(100, 100);

  EXPECT_CALL(layer, write(_, 12, _, _))
    .Times(1);

  session.read_coils(cs);
}

TEST(client_session_test, read_coils_multi_times)
{
  transport_layer_mock layer;
  client_session session(1, layer);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(1000);

  EXPECT_CALL(layer, write(_, 12, _, _))
    .Times(10);

  session.read_coils(cs);
}

TEST(client_session_test, read_coils_5_times)
{
  transport_layer_mock layer;
  client_session session(1, layer);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(5);

  EXPECT_CALL(layer, write(_, 12, _, _))
    .Times(5);

  session.read_coils(cs);
}

TEST(client_session_test, receive_response)
{
  transport_layer_mock layer;
  client c(1, layer);
  client_event_mock ce(c);
  client_session session(1, layer);
  global_client_event::instance()->event(&ce);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(layer, write(_, 12, _, _))
    .Times(6);
  EXPECT_CALL(ce, on_read_coils(_, _))
    .Times(2);

  session.read_coils(cs);
  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00};
  session.receive_response(buffer);
  buffer[1] = 0x01;
  session.receive_response(buffer);
}

TEST(client_session_test, receive_response_error)
{
  transport_layer_mock layer;
  client c(1, layer);
  client_event_mock ce(c);
  client_session session(1, layer);
  global_client_event::instance()->event(&ce);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(layer, write(_, 12, _, _))
    .Times(6);
  EXPECT_CALL(ce, on_read_coils(_, _))
    .Times(2);
  EXPECT_CALL(ce, on_error(_))
    .Times(1);

  session.read_coils(cs);
  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00};
  session.receive_response(buffer);
  buffer[1] = 0x01;
  session.receive_response(buffer);
  buffer[0] = 0x01;
  session.receive_response(buffer);
}

} // modbus_test
} // asps_test
