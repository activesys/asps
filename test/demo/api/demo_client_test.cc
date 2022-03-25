// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo client.

#include <cstdint>
#include <list>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <boost/asio.hpp>

#include <test/demo/utility/demo_test_server.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;
using namespace boost::asio;

class demo_client_test : public ::testing::Test
{
protected:
  demo_client_test()
    : server_("127.0.0.1", 9901)
  {}

  demo_test_server server_;
};

// Test demo client
TEST_F(demo_client_test, network_connect)
{
  server_.start(false);
  // The Demo client needs to know the address and port of the server
  demo_client dct("127.0.0.1", 9901);
  EXPECT_TRUE(dct.connect());
  EXPECT_TRUE(dct.is_connected());
  dct.close();
  EXPECT_FALSE(dct.is_connected());

  demo_client dcf("127.0.0.1", 9902);
  EXPECT_FALSE(dcf.connect());
  EXPECT_FALSE(dcf.is_connected());
  demo_data<uint64_t> d{1234, 1154789657886957455, 1647761782000};
  EXPECT_FALSE(dcf.send(d));

  server_.wait();
}

TEST_F(demo_client_test, send_data_of_uint64_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());

  demo_data<uint64_t> d{1234, 1154789657886957455, 1647761782000};
  EXPECT_TRUE(dc.send(d));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x22, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x08, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x10, 0x06, 0xa3, 0x13, 0x54, 0xa5, 0x9f, 0x8f // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 34);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_int32_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());

  demo_data<int32_t> d{1234, 9876, 1647761782000};
  EXPECT_TRUE(dc.send(d));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1e, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x05, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x00, 0x00, 0x26, 0x94 // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 30);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_uint16_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());

  demo_data<uint16_t> d{1234, 9876, 1647761782000};
  EXPECT_TRUE(dc.send(d));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x04, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x26, 0x94 // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 28);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_uint8_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());

  demo_data<uint8_t> d{1234, 255, 1647761782000};
  EXPECT_TRUE(dc.send(d));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x02, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0xff // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_bool_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());

  demo_data<bool> d{1234, false, 1647761782000};
  EXPECT_TRUE(dc.send(d));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x00, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x00 // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

} // demo_test
} // asps_test
