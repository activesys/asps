// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Session.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include <asps/modbus/modbus.h>
#include <test/modbus/api/transport_layer_test.h>
#include <test/modbus/api/event_test.h>

namespace asps_test {
namespace modbus_test {

using ::testing::_;
using ::testing::Return;
using namespace asps::modbus;

// TEST client session
TEST(client_session_test, mbap_header_size)
{
  EXPECT_EQ(client_session::mbap_header_size(), 7);
}

TEST(client_session_test, adu_size)
{
  uint8_t buffer[7] = {0x00, 0x01, 0x00, 0x00, 0x12, 0x34, 0x01};
  EXPECT_EQ(client_session::adu_size(buffer), 4666);
}

TEST(client_session_test, read_coils_one)
{
  client_session session(1);
  coils cs(100, 100);

  buffer_list&& buffers = session.read_coils(cs);
  EXPECT_EQ(buffers.size(), 1);
  buffer_type& buf = buffers.front();
  EXPECT_NE(buf.first, nullptr);
  EXPECT_EQ(buf.second, 12);
}

TEST(client_session_test, read_coils_multi_times)
{
  client_session session(1);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(1000);

  buffer_list&& buffers = session.read_coils(cs);
  EXPECT_EQ(buffers.size(), 10);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }
}

TEST(client_session_test, read_coils_5_times)
{
  client_session session(1);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(5);

  buffer_list&& buffers = session.read_coils(cs);
  EXPECT_EQ(buffers.size(), 5);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }
}

TEST(client_session_test, receive_response)
{
  client c(1);
  client_event_mock ce(c);
  client_transport_layer_mock layer(c);
  c.transport_layer(&layer);
  client_session session(1);
  global_client_event::instance()->event(&ce);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(ce, on_read_coils(_, _))
    .Times(2);

  buffer_list&& buffers = session.read_coils(cs);
  EXPECT_EQ(buffers.size(), 4);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }

  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00};
  buffers = session.receive_response(buffer);
  EXPECT_EQ(buffers.size(), 1);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }

  buffer[1] = 0x01;
  buffers = session.receive_response(buffer);
  EXPECT_EQ(buffers.size(), 1);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }
}

TEST(client_session_test, receive_response_error)
{
  client c(1);
  client_transport_layer_mock layer(c);
  client_event_mock ce(c);
  client_session session(1);
  global_client_event::instance()->event(&ce);
  c.transport_layer(&layer);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(ce, on_read_coils(_, _))
    .Times(2);
  EXPECT_CALL(ce, on_error(_))
    .Times(1);

  buffer_list&& buffers = session.read_coils(cs);
  EXPECT_EQ(buffers.size(), 4);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }

  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00};
  buffers = session.receive_response(buffer);
  EXPECT_EQ(buffers.size(), 1);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }

  buffer[1] = 0x01;
  buffers = session.receive_response(buffer);
  EXPECT_EQ(buffers.size(), 1);
  for (auto& buf : buffers) {
    EXPECT_NE(buf.first, nullptr);
    EXPECT_EQ(buf.second, 12);
  }

  buffer[0] = 0x01;
  buffers = session.receive_response(buffer);
  EXPECT_EQ(buffers.size(), 0);
}

// TEST server session
TEST(server_session_test, mbap_header_size)
{
  EXPECT_EQ(server_session::mbap_header_size(), 7);
}

TEST(server_session_test, adu_size)
{
  uint8_t buffer[7] = {0x00, 0x01, 0x00, 0x00, 0x12, 0x34, 0x01};
  EXPECT_EQ(server_session::adu_size(buffer), 4666);
}

TEST(server_session_test, receive_request_read_coils)
{
  server s;
  server_event_mock sem(s);
  server_transport_layer_mock stlm(s);
  s.transport_layer(&stlm);
  s.event(&sem);
  server_session session;
  uint8_t buffer[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0a};

  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(0, 10, status);
  EXPECT_CALL(sem, on_read_coils(_))
    .WillOnce(Return(cs));

  buffer_list buffers = session.receive_request(buffer);
  EXPECT_EQ(buffers.size(), 1);
  buffer_type buf = buffers.front();
  EXPECT_NE(buf.first, nullptr);
  EXPECT_EQ(buf.second, 11);
}

TEST(server_session_test, receive_request_read_coils_exception)
{
  server s;
  server_event_mock sem(s);
  server_transport_layer_mock stlm(s);
  s.transport_layer(&stlm);
  s.event(&sem);
  server_session session;
  uint8_t buffer[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0a};

  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(0, 10, status);
  cs->code(illegal_data_address);
  EXPECT_CALL(sem, on_read_coils(_))
    .WillOnce(Return(cs));

  buffer_list buffers = session.receive_request(buffer);
  EXPECT_EQ(buffers.size(), 1);
  buffer_type buf = buffers.front();
  EXPECT_NE(buf.first, nullptr);
  EXPECT_EQ(buf.second, 9);
}

} // modbus_test
} // asps_test
