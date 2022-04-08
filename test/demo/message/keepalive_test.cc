// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for keepalive.

#include <cstdint>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(positive_keepalive, serialize)
{
  positive_keepalive pka;

  const std::vector<uint8_t>& buffer = pka.serialize();
  const uint8_t expect_buffer[] = {
    0x4b, 0x45, 0x45, 0x50 // keep flag
  };

  // check length
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(positive_keepalive_ack, unserialize)
{
  positive_keepalive_ack pkaa(0x8c);
  uint8_t expect_buffer[] = {
    0x8c // ack flag
  };

  EXPECT_TRUE(pkaa.unserialize(expect_buffer));
  expect_buffer[0] = 0xff;
  EXPECT_FALSE(pkaa.unserialize(expect_buffer));
}

} // demo_test
} // asps_test
