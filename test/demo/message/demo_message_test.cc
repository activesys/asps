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
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x22, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x0a, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0xf1, 0xd6, 0x36, 0x76, 0x60, 0xe0, 0x2b, 0x41 // data value
  };

  // check length
  EXPECT_EQ(buffer.size(), 34);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(demo_message_test, serialize_float32_type)
{
  demo_data<float> v{1234, -98.12, 1647761782000};
  demo_message<float> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1e, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x09, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x71, 0x3d, 0xc4, 0xc2 // data value
  };

  // check length
  EXPECT_EQ(buffer.size(), 30);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(demo_message_test, serialize_int64_type)
{
  demo_data<int64_t> v{1234, -1154789657886957455, 1647761782000};
  demo_message<int64_t> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x22, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x07, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0xef, 0xf9, 0x5c, 0xec, 0xab, 0x5a, 0x60, 0x71 // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 34);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(demo_message_test, serialize_uint32_type)
{
  demo_data<uint32_t> v{1234, 9876, 1647761782000};
  demo_message<uint32_t> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1e, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x06, // data type
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

TEST(demo_message_test, serialize_int16_type)
{
  demo_data<int16_t> v{1234, 9876, 1647761782000};
  demo_message<int16_t> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x03, // data type
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

TEST(demo_message_test, serialize_int8_type)
{
  demo_data<int8_t> v{1234, -32, 1648001566463};
  demo_message<int8_t> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x01, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0xe0 // data value
  };

  // check length
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(demo_message_test, serialize_bool_type)
{
  demo_data<bool> v{1234, true, 1648001566463};
  demo_message<bool> msg(v);

  const std::vector<uint8_t>& buffer = msg.serialize();
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
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x01 // data value
  };
  // check length
  EXPECT_EQ(buffer.size(), 27);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

} // demo_test
} // asps_test
