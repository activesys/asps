// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo client.

#include <cstdint>
#include <list>
#include <vector>
#include <deque>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <boost/asio.hpp>

#include <asps/demo/utility/transport.h>
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
  {
    context = std::make_shared<io_context>();
  }

  demo_test_server server_;
};

// Test demo client
TEST_F(demo_client_test, network_connect_success)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {EXPECT_TRUE(success);stop();}
    void on_write(bool success, std::size_t bytes) {}
  };

  server_.start(false);
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  c.run();
  server_.wait();
}

TEST_F(demo_client_test, network_connect_fail)
{
  class connect_fail_client : public demo_client
  {
  public:
    void on_connect(bool success) {EXPECT_FALSE(success);stop();}
    void on_write(bool success, std::size_t bytes) {}
  };

  config::address("127.0.0.1", 9902);
  connect_fail_client c;
  data_group_type p{
    make_demo_data<uint64_t>(1234, 1154789657886957455, 1647761782000)
  };
  EXPECT_FALSE(c.send(p));
  c.run();
}

TEST_F(demo_client_test, send_data_of_uint64_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<uint64_t>(1234, 1154789657886957455, 1647761782000)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 34);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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

  // check
  EXPECT_EQ(buffer.size(), 34);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_int32_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1234, 9876, 1647761782000)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 30);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 30);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_uint16_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<uint16_t>(1234, 9876, 1647761782000)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 28);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 28);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_uint8_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<uint8_t>(1234, 255, 1647761782000)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 27);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_data_of_bool_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<bool>(1234, false, 1647761782000)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 27);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_int32_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1001, 9001, 1648363193268),
        make_demo_data<int32_t>(1002, 9002, 1648363193268),
        make_demo_data<int32_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 81);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 81);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_types)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1001, 9001, 1648363193268),
        make_demo_data<int16_t>(1002, 9002, 1648363193268),
        make_demo_data<uint16_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 77);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 77);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1002, 9001, 1648363193268),
        make_demo_data<int32_t>(1004, 9002, 1648363193268),
        make_demo_data<int32_t>(1006, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 78);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_type(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 78);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_same_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1002, 9001, 1648363193268),
        make_demo_data<int32_t>(1004, 9002, 1648363193268),
        make_demo_data<uint32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1002, 9001, 1648363193268),
        make_demo_data<int32_t>(1006, 9003, 1648363193268),
        make_demo_data<uint16_t>(1004, 9002, 1648363193268),
        make_demo_data<uint32_t>(1006, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 168);
      stop();
    }
  };

  server_.expect_length(168);
  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_type(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 168);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1002, 9001, 1648363193268),
        make_demo_data<int16_t>(1004, 9002, 1648363193268),
        make_demo_data<uint16_t>(1006, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 53);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 53);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_same_timestamp)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1002, 9001, 1648363193268),
        make_demo_data<int32_t>(1000, 9000, 1648363193270),
        make_demo_data<uint32_t>(1002, 9001, 1648363193270),
        make_demo_data<int16_t>(1004, 9002, 1648363193268),
        make_demo_data<uint16_t>(1006, 9003, 1648363193268),
        make_demo_data<int16_t>(1004, 9002, 1648363193270),
        make_demo_data<uint16_t>(1006, 9003, 1648363193270)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 106);
      stop();
    }
  };

  server_.expect_length(106);
  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 106);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_key_sequence)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1001, 9001, 1648363193269),
        make_demo_data<int16_t>(1002, 9002, 1648363193270),
        make_demo_data<uint16_t>(1003, 9003, 1648363193271)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 65);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 65);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_multiple_packages_containing_key_sequence)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
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
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 180);
      stop();
    }
  };

  server_.expect_length(180);
  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 180);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp_and_key_sequence)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1001, 9001, 1648363193268),
        make_demo_data<int16_t>(1002, 9002, 1648363193268),
        make_demo_data<uint16_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 41);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  config::key_sequence(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 41);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_same_timestamp_and_same_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1001, 9001, 1648363193268),
        make_demo_data<int32_t>(1002, 9002, 1648363193268),
        make_demo_data<int32_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 54);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::same_timestamp(true);
  config::same_type(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 54);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_key_sequence_and_same_type)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1001, 9001, 1648363193268),
        make_demo_data<int32_t>(1002, 9002, 1648363193268),
        make_demo_data<int32_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 66);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  config::key_sequence(true);
  config::same_type(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 66);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_datas_of_all_attribute)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1001, 9001, 1648363193268),
        make_demo_data<int32_t>(1002, 9002, 1648363193268),
        make_demo_data<int32_t>(1003, 9003, 1648363193268)
      };
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 42);
      stop();
    }
  };

  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(true);
  config::key_sequence(true);
  config::same_timestamp(true);
  c.run();
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
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      data_group_type p{
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
      EXPECT_TRUE(send(p));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 126);
      stop();
    }
  };

  server_.expect_length(126);
  server_.start();
  config::address("127.0.0.1", 9901);
  connect_success_client c;
  config::same_type(true);
  config::key_sequence(true);
  config::same_timestamp(true);
  c.run();
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
  // check
  EXPECT_EQ(buffer.size(), 126);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, send_positive_keepalive)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success) {
      EXPECT_TRUE(success);
      std::this_thread::sleep_for(std::chrono::seconds(config::t1()+1));
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 4);
      stop();
    }
  };
  config::t1_t2(2,1);
  config::pack_nkeep(0xff, 0x00);
  config::address("127.0.0.1", 9901);
  server_.expect_length(4);
  server_.start();
  connect_success_client c;
  c.run();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    0x4b, 0x45, 0x45, 0x50 // KEEP flag
  };
  // check
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, t0_t1_t2)
{
  class connect_success_client : public demo_client
  {
  public:
    connect_success_client()
      : demo_client(),
        reconnect_(false)
    {}

  public:
    void on_connect(bool success) {
      if (!reconnect_) {
        EXPECT_TRUE(success);
        std::this_thread::sleep_for(std::chrono::seconds(config::t1()+1));
        reconnect_ = true;
      } else {
        EXPECT_TRUE(success);
        stop();
      }
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 4);
    }

  private:
    bool reconnect_;
  };
  config::t0(3);
  config::t1_t2(2,1);
  config::pack_nkeep(0xff, 0x00);
  config::address("127.0.0.1", 9901);
  server_.expect_length(4);
  server_.start();
  connect_success_client c;
  c.run();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    0x4b, 0x45, 0x45, 0x50 // KEEP flag
  };
  // check
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, call_t0_multiple_times)
{
  class connect_fail_client : public demo_client
  {
  public:
    connect_fail_client()
      : demo_client(),
        reconnect_(false)
    {}

  public:
    void on_connect(bool success) {
      if (!reconnect_) {
        EXPECT_FALSE(success);
        reconnect_ = true;
      } else {
        EXPECT_FALSE(success);
        stop();
      }
    }
    void on_write(bool success, std::size_t bytes)
    {}

  private:
    bool reconnect_;
  };

  config::t0(2);
  config::address("127.0.0.1", 9902);
  connect_fail_client c;
  c.run();
}

TEST_F(demo_client_test, negative_keepalive)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success)
    {
      EXPECT_TRUE(success);
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 4);
      stop();
    }
  };
  config::t0(3);
  config::t1_t2(2,1);
  config::pack_nkeep(0xff, 0x00);
  config::address("127.0.0.1", 9901);
  std::vector<uint8_t> nkeep{config::nkeep()};
  server_.write_data(nkeep);
  server_.expect_length(4);
  server_.start();
  connect_success_client c;
  c.run();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    0x4b, 0x41, 0x43, 0x4b // KACK flag
  };
  // check
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST_F(demo_client_test, pack_and_nkeep)
{
  class connect_success_client : public demo_client
  {
  public:
    void on_connect(bool success)
    {
      EXPECT_TRUE(success);
    }
    void on_write(bool success, std::size_t bytes)
    {
      EXPECT_TRUE(success);
      EXPECT_EQ(bytes, 4);
      stop();
    }
  };
  config::t0(3);
  config::t1_t2(2,1);
  config::pack_nkeep(0xff, 0x00);
  config::address("127.0.0.1", 9901);
  std::vector<uint8_t> datas{config::pack(), 'K', 'E', 'E', 'P', config::nkeep(), 'D', 'E', 'M', 'O'};
  server_.write_data(datas);
  server_.expect_length(4);
  server_.start();
  connect_success_client c;
  c.run();
  server_.wait();

  const std::vector<uint8_t>& buffer = server_.message();
  const uint8_t expect_buffer[] = {
    0x4b, 0x41, 0x43, 0x4b // KACK flag
  };
  // check
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

} // demo_test
} // asps_test
