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
using namespace asps::modbus;

class write_interface
{
public:
  virtual void write(const uint8_t* buffer, std::size_t length) = 0;
};
class write_mock : public write_interface
{
public:
  MOCK_METHOD(void, write, (const uint8_t*, std::size_t), (override));
};

// TEST client session
TEST(client_session_test, mbap_header_size)
{
  write_mock wm;
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  EXPECT_EQ(session.mbap_header_size(), 7);
}

TEST(client_session_test, adu_size)
{
  write_mock wm;
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  uint8_t buffer[7] = {0x00, 0x01, 0x00, 0x00, 0x12, 0x34, 0x01};
  EXPECT_EQ(session.adu_size(buffer), 4666);
}

TEST(client_session_test, read_coils_one)
{
  write_mock wm;
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  coils cs(100, 100);

  EXPECT_CALL(wm, write(_, 12))
    .Times(1);

  session.read_coils(cs);
}

TEST(client_session_test, read_coils_multi_times)
{
  write_mock wm;
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(1000);

  EXPECT_CALL(wm, write(_, 12))
    .Times(10);

  session.read_coils(cs);
}

TEST(client_session_test, read_coils_5_times)
{
  write_mock wm;
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(5);

  EXPECT_CALL(wm, write(_, 12))
    .Times(5);

  session.read_coils(cs);
}

TEST(client_session_test, receive_response)
{
  write_mock wm;
  transport_layer_mock layer;
  client c(1, layer);
  client_event_mock ce(c);
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  global_client_event::instance()->event(&ce);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(wm, write(_, 12))
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
  write_mock wm;
  transport_layer_mock layer;
  client c(1, layer);
  client_event_mock ce(c);
  client_session session(1, std::bind(&write_mock::write, &wm, _1, _2));
  global_client_event::instance()->event(&ce);
  coils cs(100, 100);
  config::quantity_of_coils(10);
  config::quantity_of_concurrent_requests(4);

  EXPECT_CALL(wm, write(_, 12))
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
