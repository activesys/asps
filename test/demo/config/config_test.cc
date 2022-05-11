// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for config.

#include <cstdint>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(config_test, t0_t1_t2)
{
  config::t0(80);
  EXPECT_EQ(config::t0(), 80);
  config::t1_t2(30, 20);
  EXPECT_EQ(config::t1(), 30);
  EXPECT_EQ(config::t2(), 20);
  EXPECT_TRUE(config::t1_t2(100, 78));
  EXPECT_EQ(config::t1(), 100);
  EXPECT_EQ(config::t2(), 78);
  EXPECT_FALSE(config::t1_t2(78, 100));
  EXPECT_EQ(config::t1(), 100);
  EXPECT_EQ(config::t2(), 78);
}

TEST(config_test, pack_nkeep)
{
  config::pack_nkeep(0xff, 0x00);
  EXPECT_EQ(config::pack(), 0xff);
  EXPECT_EQ(config::nkeep(), 0x00);
  EXPECT_TRUE(config::pack_nkeep(0x6a, 0xcd));
  EXPECT_EQ(config::pack(), 0x6a);
  EXPECT_EQ(config::nkeep(), 0xcd);
  EXPECT_FALSE(config::pack_nkeep(0xaa, 0xaa));
  EXPECT_EQ(config::pack(), 0x6a);
  EXPECT_EQ(config::nkeep(), 0xcd);
}

TEST(config_test, compress_attributes)
{
  config::same_type(false);
  config::key_sequence(false);
  config::same_timestamp(false);
  EXPECT_FALSE(config::same_type());
  config::same_type(true);
  EXPECT_TRUE(config::same_type());
  EXPECT_FALSE(config::key_sequence());
  config::key_sequence(true);
  EXPECT_TRUE(config::key_sequence());
  EXPECT_FALSE(config::same_timestamp());
  config::same_timestamp(true);
  EXPECT_TRUE(config::same_timestamp());
}

} // demo_test
} // asps_test
