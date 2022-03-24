// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo message.

#include <cstdint>
#include <vector>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(demo_message_test, serialize_float64_type)
{
  demo_data<double> v{1234, 913456.230887143, 1647761782000};
  demo_message<double> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x0b);
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
  EXPECT_EQ(buffer[24], 0xf1);
  EXPECT_EQ(buffer[25], 0xd6);
  EXPECT_EQ(buffer[26], 0x36);
  EXPECT_EQ(buffer[27], 0x76);
  EXPECT_EQ(buffer[28], 0x60);
  EXPECT_EQ(buffer[29], 0xe0);
  EXPECT_EQ(buffer[30], 0x2b);
  EXPECT_EQ(buffer[31], 0x41);
}

TEST(demo_message_test, serialize_float32_type)
{
  demo_data<float> v{1234, -98.12, 1647761782000};
  demo_message<float> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x0a);
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
  EXPECT_EQ(buffer[24], 0x71);
  EXPECT_EQ(buffer[25], 0x3d);
  EXPECT_EQ(buffer[26], 0xc4);
  EXPECT_EQ(buffer[27], 0xc2);
}

TEST(demo_message_test, serialize_int64_type)
{
  demo_data<int64_t> v{1234, -1154789657886957455, 1647761782000};
  demo_message<int64_t> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x07);
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
  EXPECT_EQ(buffer[24], 0xef);
  EXPECT_EQ(buffer[25], 0xf9);
  EXPECT_EQ(buffer[26], 0x5c);
  EXPECT_EQ(buffer[27], 0xec);
  EXPECT_EQ(buffer[28], 0xab);
  EXPECT_EQ(buffer[29], 0x5a);
  EXPECT_EQ(buffer[30], 0x60);
  EXPECT_EQ(buffer[31], 0x71);
}

TEST(demo_message_test, serialize_uint32_type)
{
  demo_data<uint32_t> v{1234, 9876, 1647761782000};
  demo_message<uint32_t> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x06);
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

TEST(demo_message_test, serialize_int16_type)
{
  demo_data<int16_t> v{1234, 9876, 1647761782000};
  demo_message<int16_t> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x03);
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

TEST(demo_message_test, serialize_int8_type)
{
  demo_data<int8_t> v{1234, -32, 1648001566463};
  demo_message<int8_t> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[12], 0x01);
  // check key field
  EXPECT_EQ(buffer[13], 0x00);
  EXPECT_EQ(buffer[14], 0x04);
  EXPECT_EQ(buffer[15], 0xd2);
  // check timestamp field
  EXPECT_EQ(buffer[16], 0x00);
  EXPECT_EQ(buffer[17], 0x00);
  EXPECT_EQ(buffer[18], 0x01);
  EXPECT_EQ(buffer[19], 0x7f);
  EXPECT_EQ(buffer[20], 0xb4);
  EXPECT_EQ(buffer[21], 0x8c);
  EXPECT_EQ(buffer[22], 0x46);
  EXPECT_EQ(buffer[23], 0xff);
  // check value field
  EXPECT_EQ(buffer[24], 0xe0);
}

TEST(demo_message_test, serialize_bool_type)
{
  demo_data<bool> v{1234, true, 1648001566463};
  demo_message<bool> msg(v);

 const std::vector<uint8_t>& buffer = msg.serialize();
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
  EXPECT_EQ(buffer[20], 0xb4);
  EXPECT_EQ(buffer[21], 0x8c);
  EXPECT_EQ(buffer[22], 0x46);
  EXPECT_EQ(buffer[23], 0xff);
  // check value field
  EXPECT_EQ(buffer[24], 0x01);
}


} // demo_test
} // asps_test
