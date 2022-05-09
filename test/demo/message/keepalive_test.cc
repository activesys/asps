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

TEST(client_positive_keepalive, serialize)
{
  client_positive_keepalive pka;

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

TEST(client_positive_keepalive_ack, unserialize)
{
  config::pack_nkeep(0x8c, 0x00);
  client_positive_keepalive_ack pkaa;
  std::vector<uint8_t> expect_buffer{
    0x8c // ack flag
  };

  EXPECT_TRUE(pkaa.unserialize(expect_buffer));
  expect_buffer[0] = 0xff;
  EXPECT_FALSE(pkaa.unserialize(expect_buffer));
}

TEST(client_negative_keepalive, unserialize)
{
  config::pack_nkeep(0xff, 0x6b);
  client_negative_keepalive nka;
  std::vector<uint8_t> expect_buffer{
    0x6b // ack flag
  };

  EXPECT_TRUE(nka.unserialize(expect_buffer));
  expect_buffer[0] = 0x00;
  EXPECT_FALSE(nka.unserialize(expect_buffer));
}

TEST(client_negative_keepalive_ack, serialize)
{
  client_negative_keepalive_ack nkaa;

  const std::vector<uint8_t>& buffer = nkaa.serialize();
  const uint8_t expect_buffer[] = {
    0x4b, 0x41, 0x43, 0x4b // kack flag
  };

  // check length
  EXPECT_EQ(buffer.size(), 4);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(server_positive_keepalive, unserialize)
{
  server_positive_keepalive pka;
  std::vector<uint8_t> buffer{
    0x4b, 0x45, 0x45, 0x50 // keep flag
  };

  EXPECT_TRUE(pka.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
}

TEST(server_positive_keepalive_ack, serialize)
{
  config::pack_nkeep(0xff, 0x6b);
  server_positive_keepalive_ack pkaa;

  const std::vector<uint8_t>& buffer = pkaa.serialize();
  const uint8_t expect_buffer[] = {
    0xff // ack flag
  };

  // check length
  EXPECT_EQ(buffer.size(), 1);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(server_negative_keepalive, serialize)
{
  config::pack_nkeep(0x8c, 0x00);
  server_negative_keepalive nka;

  const std::vector<uint8_t>& buffer = nka.serialize();
  const uint8_t expect_buffer[] = {
    0x00
  };

  // check length
  EXPECT_EQ(buffer.size(), 1);
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(server_negative_keepalive_ack, unserialize)
{
  server_negative_keepalive_ack nkaa;
  std::vector<uint8_t> expect_buffer{
    0x4c, 0x41, 0x43, 0x4b // kack flag
  };

  EXPECT_FALSE(nkaa.unserialize(expect_buffer));
  expect_buffer[0] = 0x4b;
  EXPECT_TRUE(nkaa.unserialize(expect_buffer));
}

} // demo_test
} // asps_test
