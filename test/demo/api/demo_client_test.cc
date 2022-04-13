// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo client.

#include <cstdint>
#include <list>
#include <vector>
#include <deque>
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
  data_group_type p{
    make_demo_data<uint64_t>(1234, 1154789657886957455, 1647761782000)
  };
  EXPECT_FALSE(dcf.send(p));

  server_.wait();
}

TEST_F(demo_client_test, send_data_of_uint64_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type p{
    make_demo_data<uint64_t>(1234, 1154789657886957455, 1647761782000)
  };
  EXPECT_TRUE(dc.send(p));
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
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type p{
    make_demo_data<int32_t>(1234, 9876, 1647761782000)
  };
  EXPECT_TRUE(dc.send(p));
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
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type p{
    make_demo_data<uint16_t>(1234, 9876, 1647761782000)
  };
  EXPECT_TRUE(dc.send(p));
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
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type p{
    make_demo_data<uint8_t>(1234, 255, 1647761782000)
  };
  EXPECT_TRUE(dc.send(p));
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
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type p{
    make_demo_data<bool>(1234, false, 1647761782000)
  };
  EXPECT_TRUE(dc.send(p));
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

TEST_F(demo_client_test, send_datas_of_int32_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1001, 9001, 1648363193268),
    make_demo_data<int32_t>(1002, 9002, 1648363193268),
    make_demo_data<int32_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x51, // header length
    0x00, 0x04, // header count
    0x00, // header attr
    // mutable
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x05, // data type
    0x00, 0x00, 0x03, 0xe9, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x05, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x05, // data type
    0x00, 0x00, 0x03, 0xeb, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 81);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_types)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1001, 9001, 1648363193268),
    make_demo_data<int16_t>(1002, 9002, 1648363193268),
    make_demo_data<uint16_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x4d, // header length
    0x00, 0x04, // header count
    0x00, // header attr
    // mutable
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xe9, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xeb, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 77);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_type(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1002, 9001, 1648363193268),
    make_demo_data<int32_t>(1004, 9002, 1648363193268),
    make_demo_data<int32_t>(1006, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x4e, // header length
    0x00, 0x04, // header count
    0x01, // header attr
    // mutable
    0x05,
    // data 1
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x03, 0xee, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 78);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_same_type)
{
  server_.expect_length(168);
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_type(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1002, 9001, 1648363193268),
    make_demo_data<int32_t>(1004, 9002, 1648363193268),
    make_demo_data<uint32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1002, 9001, 1648363193268),
    make_demo_data<int32_t>(1006, 9003, 1648363193268),
    make_demo_data<uint16_t>(1004, 9002, 1648363193268),
    make_demo_data<uint32_t>(1006, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // package 1: k uint16_t
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x01, // header count
    0x01, // header attr
    // mutable
    0x04,
    // data 1
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x23, 0x2a, // data value
    // package 2: k int32_t
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x4e, // header length
    0x00, 0x04, // header count
    0x01, // header attr
    // mutable
    0x05,
    // data 1
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x03, 0xee, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b, // data value
    // package 2: k uint32_t
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x3e, // header length
    0x00, 0x03, // header count
    0x01, // header attr
    // mutable
    0x06,
    // data 1
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 4
    0x00, 0x00, 0x03, 0xee, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 168);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1002, 9001, 1648363193268),
    make_demo_data<int16_t>(1004, 9002, 1648363193268),
    make_demo_data<uint16_t>(1006, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x35, // header length
    0x00, 0x04, // header count
    0x04, // header attr
    // mutable
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xee, // data key
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 53);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_same_timestamp)
{
  server_.expect_length(106);
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1002, 9001, 1648363193268),
    make_demo_data<int32_t>(1000, 9000, 1648363193270),
    make_demo_data<uint32_t>(1002, 9001, 1648363193270),
    make_demo_data<int16_t>(1004, 9002, 1648363193268),
    make_demo_data<uint16_t>(1006, 9003, 1648363193268),
    make_demo_data<int16_t>(1004, 9002, 1648363193270),
    make_demo_data<uint16_t>(1006, 9003, 1648363193270)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // package 1: t 1648363193268
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x35, // header length
    0x00, 0x04, // header count
    0x04, // header attr
    // mutable
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xee, // data key
    0x23, 0x2b, // data value
    // package 2: t 1648363193270
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x35, // header length
    0x00, 0x04, // header count
    0x04, // header attr
    // mutable
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xee, // data key
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 106);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_key_sequence)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1001, 9001, 1648363193269),
    make_demo_data<int16_t>(1002, 9002, 1648363193270),
    make_demo_data<uint16_t>(1003, 9003, 1648363193271)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x41, // header length
    0x00, 0x04, // header count
    0x02, // header attr
    // mutable
    0x00, 0x00, 0x03, 0xe8, // data key
    // data 1
    0x05, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb5, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 65);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_key_sequence)
{
  server_.expect_length(180);
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1001, 9001, 1648363193269),
    make_demo_data<int16_t>(1002, 9002, 1648363193270),
    make_demo_data<uint16_t>(1003, 9003, 1648363193271),
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1001, 9001, 1648363193269),
    make_demo_data<int16_t>(1002, 9002, 1648363193270),
    make_demo_data<uint16_t>(1003, 9003, 1648363193271),
    make_demo_data<int16_t>(1004, 9002, 1648363193270),
    make_demo_data<uint16_t>(999, 9003, 1648363193271),
    make_demo_data<uint16_t>(9999, 9003, 1648363193271)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // package 1: k 999 ~ 1004
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x57, // header length
    0x00, 0x06, // header count
    0x02, // header attr
    // mutable
    0x00, 0x00, 0x03, 0xe7, // data key
    // data 1
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b, // data value
    // data 2
    0x05, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 3
    0x06, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb5, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 4
    0x03, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    0x23, 0x2a, // data value
    // data 5
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b, // data value
    // data 6
    0x03, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    0x23, 0x2a, // data value
    // pacakge 2: k 1000 ~ 1003
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x41, // header length
    0x00, 0x04, // header count
    0x02, // header attr
    // mutable
    0x00, 0x00, 0x03, 0xe8, // data key
    // data 1
    0x05, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb5, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b, // data value
    // pacakge 3: k 9999
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x01, // header count
    0x02, // header attr
    // mutable
    0x00, 0x00, 0x27, 0x0f, // data key
    // data 1
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 180);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp_and_key_sequence)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  config::key_sequence(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1001, 9001, 1648363193268),
    make_demo_data<int16_t>(1002, 9002, 1648363193268),
    make_demo_data<uint16_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x29, // header length
    0x00, 0x04, // header count
    0x06, // header attr
    // mutable
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x05, // data type
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 41);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp_and_same_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  config::same_type(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1001, 9001, 1648363193268),
    make_demo_data<int32_t>(1002, 9002, 1648363193268),
    make_demo_data<int32_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x36, // header length
    0x00, 0x04, // header count
    0x05, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x03, 0xe9, // data key
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x03, 0xeb, // data key
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 54);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_key_sequence_and_same_type)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);
  config::same_type(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1001, 9001, 1648363193268),
    make_demo_data<int32_t>(1002, 9002, 1648363193268),
    make_demo_data<int32_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x42, // header length
    0x00, 0x04, // header count
    0x03, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    // data 1
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 66);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_all_attribute)
{
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(true);
  config::key_sequence(true);
  config::same_timestamp(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<int32_t>(1001, 9001, 1648363193268),
    make_demo_data<int32_t>(1002, 9002, 1648363193268),
    make_demo_data<int32_t>(1003, 9003, 1648363193268)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 42);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_all_attribute_multiple_packages)
{
  server_.expect_length(126);
  server_.start();

  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  config::same_type(true);
  config::key_sequence(true);
  config::same_timestamp(true);

  data_group_type ds{
    make_demo_data<int32_t>(1000, 9000, 1648363193268),
    make_demo_data<uint32_t>(1000, 9000, 1648363193270),
    make_demo_data<int32_t>(1002, 9002, 1648363193268),
    make_demo_data<int32_t>(1004, 9000, 1648363193270),
    make_demo_data<uint32_t>(1002, 9002, 1648363193270),
    make_demo_data<int32_t>(1006, 9002, 1648363193270),
    make_demo_data<uint32_t>(1001, 9001, 1648363193270),
    make_demo_data<int32_t>(1003, 9003, 1648363193268),
    make_demo_data<uint32_t>(1003, 9003, 1648363193270),
    make_demo_data<int32_t>(1005, 9001, 1648363193270),
    make_demo_data<int32_t>(1001, 9001, 1648363193268),
    make_demo_data<int32_t>(1007, 9003, 1648363193270)
  };
  EXPECT_TRUE(dc.send(ds));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    // package 1: t int32_t, k 1000, T 1648363193268
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b, // data value
    // package 2: t int32_t, k 1004, T 1648363193270
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b, // data value
    // package 3: t uint32_t, k 1000, T 1648363193270
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x06, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 126);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, receive_negative_keepalive)
{
  config::pack_nkeep(0xff, 0x00);
  server_.expect_length(4);
  server_.start();
  demo_client dc("127.0.0.1", 9901);
  dc.connect();
  EXPECT_TRUE(dc.is_connected());
  uint8_t expect_buffer_ack[] = {0x00};
  EXPECT_TRUE(dc.receive(expect_buffer_ack));
  dc.close();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    0x4b, 0x41, 0x43, 0x4b // kack flag
  };
  // check length
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

} // demo_test
} // asps_test
