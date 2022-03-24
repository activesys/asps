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
  {
  }

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
  // check length
  EXPECT_EQ(buffer.size(), 32);
  // check header
  EXPECT_EQ(buffer[0], 0x44);
  EXPECT_EQ(buffer[1], 0x45);
  EXPECT_EQ(buffer[2], 0x4d);
  EXPECT_EQ(buffer[3], 0x4f);
  // check length field
  EXPECT_EQ(buffer[4], 0x00);
  EXPECT_EQ(buffer[5], 0x20);
  // check count field
  EXPECT_EQ(buffer[6], 0x00);
  EXPECT_EQ(buffer[7], 0x01);
  // check attribute
  EXPECT_EQ(buffer[8], 0x00);
  EXPECT_EQ(buffer[9], 0x00);
  EXPECT_EQ(buffer[10], 0x00);
  EXPECT_EQ(buffer[11], 0x00);
  // check data
  // check type field
  EXPECT_EQ(buffer[12], 0x08);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xa6);
  EXPECT_EQ(buffer[21], 0x41);
  EXPECT_EQ(buffer[22], 0x74);
  EXPECT_EQ(buffer[23], 0xf0);
  // check value field
  EXPECT_EQ(buffer[24], 0x10);
  EXPECT_EQ(buffer[25], 0x06);
  EXPECT_EQ(buffer[26], 0xa3);
  EXPECT_EQ(buffer[27], 0x13);
  EXPECT_EQ(buffer[28], 0x54);
  EXPECT_EQ(buffer[29], 0xa5);
  EXPECT_EQ(buffer[30], 0x9f);
  EXPECT_EQ(buffer[31], 0x8f);
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
  // check length
  EXPECT_EQ(buffer.size(), 28);
  // check header
  EXPECT_EQ(buffer[0], 0x44);
  EXPECT_EQ(buffer[1], 0x45);
  EXPECT_EQ(buffer[2], 0x4d);
  EXPECT_EQ(buffer[3], 0x4f);
  // check length field
  EXPECT_EQ(buffer[4], 0x00);
  EXPECT_EQ(buffer[5], 0x1c);
  // check count field
  EXPECT_EQ(buffer[6], 0x00);
  EXPECT_EQ(buffer[7], 0x01);
  // check attribute
  EXPECT_EQ(buffer[8], 0x00);
  EXPECT_EQ(buffer[9], 0x00);
  EXPECT_EQ(buffer[10], 0x00);
  EXPECT_EQ(buffer[11], 0x00);
  // check data
  // check type field
  EXPECT_EQ(buffer[12], 0x05);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xa6);
  EXPECT_EQ(buffer[21], 0x41);
  EXPECT_EQ(buffer[22], 0x74);
  EXPECT_EQ(buffer[23], 0xf0);
  // check value field
  EXPECT_EQ(buffer[24], 0x00);
  EXPECT_EQ(buffer[25], 0x00);
  EXPECT_EQ(buffer[26], 0x26);
  EXPECT_EQ(buffer[27], 0x94);
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
  // check length
  EXPECT_EQ(buffer.size(), 26);
  // check header
  EXPECT_EQ(buffer[0], 0x44);
  EXPECT_EQ(buffer[1], 0x45);
  EXPECT_EQ(buffer[2], 0x4d);
  EXPECT_EQ(buffer[3], 0x4f);
  // check length field
  EXPECT_EQ(buffer[4], 0x00);
  EXPECT_EQ(buffer[5], 0x1a);
  // check count field
  EXPECT_EQ(buffer[6], 0x00);
  EXPECT_EQ(buffer[7], 0x01);
  // check attribute
  EXPECT_EQ(buffer[8], 0x00);
  EXPECT_EQ(buffer[9], 0x00);
  EXPECT_EQ(buffer[10], 0x00);
  EXPECT_EQ(buffer[11], 0x00);
  // check data
  // check type field
  EXPECT_EQ(buffer[12], 0x04);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xa6);
  EXPECT_EQ(buffer[21], 0x41);
  EXPECT_EQ(buffer[22], 0x74);
  EXPECT_EQ(buffer[23], 0xf0);
  // check value field
  EXPECT_EQ(buffer[24], 0x26);
  EXPECT_EQ(buffer[25], 0x94);
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
  // check length
  EXPECT_EQ(buffer.size(), 25);
  // check header
  EXPECT_EQ(buffer[0], 0x44);
  EXPECT_EQ(buffer[1], 0x45);
  EXPECT_EQ(buffer[2], 0x4d);
  EXPECT_EQ(buffer[3], 0x4f);
  // check length field
  EXPECT_EQ(buffer[4], 0x00);
  EXPECT_EQ(buffer[5], 0x19);
  // check count field
  EXPECT_EQ(buffer[6], 0x00);
  EXPECT_EQ(buffer[7], 0x01);
  // check attribute
  EXPECT_EQ(buffer[8], 0x00);
  EXPECT_EQ(buffer[9], 0x00);
  EXPECT_EQ(buffer[10], 0x00);
  EXPECT_EQ(buffer[11], 0x00);
  // check data
  // check type field
  EXPECT_EQ(buffer[12], 0x02);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xa6);
  EXPECT_EQ(buffer[21], 0x41);
  EXPECT_EQ(buffer[22], 0x74);
  EXPECT_EQ(buffer[23], 0xf0);
  // check value field
  EXPECT_EQ(buffer[24], 0xff);
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
  // check length
  EXPECT_EQ(buffer.size(), 25);
  // check header
  EXPECT_EQ(buffer[0], 0x44);
  EXPECT_EQ(buffer[1], 0x45);
  EXPECT_EQ(buffer[2], 0x4d);
  EXPECT_EQ(buffer[3], 0x4f);
  // check length field
  EXPECT_EQ(buffer[4], 0x00);
  EXPECT_EQ(buffer[5], 0x19);
  // check count field
  EXPECT_EQ(buffer[6], 0x00);
  EXPECT_EQ(buffer[7], 0x01);
  // check attribute
  EXPECT_EQ(buffer[8], 0x00);
  EXPECT_EQ(buffer[9], 0x00);
  EXPECT_EQ(buffer[10], 0x00);
  EXPECT_EQ(buffer[11], 0x00);
  // check data
  // check type field
  EXPECT_EQ(buffer[12], 0x00);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xa6);
  EXPECT_EQ(buffer[21], 0x41);
  EXPECT_EQ(buffer[22], 0x74);
  EXPECT_EQ(buffer[23], 0xf0);
  // check value field
  EXPECT_EQ(buffer[24], 0x00);
}

} // demo_test
} // asps_test
