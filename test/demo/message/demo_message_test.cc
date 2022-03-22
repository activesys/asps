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

} // demo_test
} // asps_test
