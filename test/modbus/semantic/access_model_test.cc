// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Access Model.

#include <gtest/gtest.h>
#include <asps/modbus/semantic/access_model.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test Access Model
TEST(access_model_test, read_only_model_split)
{
  uint16_t datas[30] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
  read_only_model<uint16_t> model(100, 30, datas);

  for (int i = 0; i < 30; ++i) {
    EXPECT_EQ(model[i+100], i);
  }
  EXPECT_THROW(model.at(99), std::out_of_range);
  EXPECT_THROW(model.at(130), std::out_of_range);

  std::deque<read_only_model<uint16_t>::ptr_type> q;
  read_only_model<uint16_t>::split(model, 10, q);
  EXPECT_EQ(q.size(), 3);
  int i = 0;
  for (std::deque<read_only_model<uint16_t>::ptr_type>::iterator it = q.begin();
       it != q.end();
       ++it, ++i) {
    for (int j = 0; j < 10; ++j) {
      EXPECT_EQ((*it)->at(j + i * 10 + 100), j + i * 10);
    }
    EXPECT_THROW((*it)->at(i * 10 + 100 - 1), std::out_of_range);
    EXPECT_THROW((*it)->at((i+1) * 10 + 100), std::out_of_range);
  }
}

TEST(access_model_test, read_write_model_split)
{
  bool datas[30] = {
    false, true, true, false, false, false, true, true,
    true, true, true, true, true, true, false, true, 
    true, true, true, true, false, false, false, false, 
    false, true, true, false, false, true};
  read_write_model<bool> model(100, 30, datas);

  std::deque<read_write_model<bool>::ptr_type> q;
  read_write_model<bool>::split(model, 8, q);
  EXPECT_EQ(q.size(), 4);
  std::deque<read_write_model<bool>::ptr_type>::iterator it = q.begin();
  // the first 8 elements
  EXPECT_FALSE((*it)->at(100));
  EXPECT_TRUE((*it)->at(101));
  EXPECT_TRUE((*it)->at(102));
  EXPECT_FALSE((*it)->at(103));
  EXPECT_FALSE((*it)->at(104));
  EXPECT_FALSE((*it)->at(105));
  EXPECT_TRUE((*it)->at(106));
  EXPECT_TRUE((*it)->at(107));
  EXPECT_THROW((*it)->at(99), std::out_of_range);
  EXPECT_THROW((*it)->at(108), std::out_of_range);
  (*it)->at(103) = true;
  EXPECT_TRUE((*it)->at(103));
  EXPECT_TRUE(model.at(103));
  ++it;
  // the secode 8 elements
  EXPECT_TRUE((*it)->at(108));
  EXPECT_TRUE((*it)->at(109));
  EXPECT_TRUE((*it)->at(110));
  EXPECT_TRUE((*it)->at(111));
  EXPECT_TRUE((*it)->at(112));
  EXPECT_TRUE((*it)->at(113));
  EXPECT_FALSE((*it)->at(114));
  EXPECT_TRUE((*it)->at(115));
  EXPECT_THROW((*it)->at(107), std::out_of_range);
  EXPECT_THROW((*it)->at(116), std::out_of_range);
  (*it)->at(114) = true;
  EXPECT_TRUE((*it)->at(114));
  EXPECT_TRUE(model.at(114));
  ++it;
  // the third 8 elements
  EXPECT_TRUE((*it)->at(116));
  EXPECT_TRUE((*it)->at(117));
  EXPECT_TRUE((*it)->at(118));
  EXPECT_TRUE((*it)->at(119));
  EXPECT_FALSE((*it)->at(120));
  EXPECT_FALSE((*it)->at(121));
  EXPECT_FALSE((*it)->at(122));
  EXPECT_FALSE((*it)->at(123));
  EXPECT_THROW((*it)->at(115), std::out_of_range);
  EXPECT_THROW((*it)->at(124), std::out_of_range);
  (*it)->at(118) = false;
  EXPECT_FALSE((*it)->at(118));
  EXPECT_FALSE(model.at(118));
  ++it;
  // the last 6 elements
  EXPECT_FALSE((*it)->at(124));
  EXPECT_TRUE((*it)->at(125));
  EXPECT_TRUE((*it)->at(126));
  EXPECT_FALSE((*it)->at(127));
  EXPECT_FALSE((*it)->at(128));
  EXPECT_TRUE((*it)->at(129));
  EXPECT_THROW((*it)->at(123), std::out_of_range);
  EXPECT_THROW((*it)->at(130), std::out_of_range);
  (*it)->at(129) = false;
  EXPECT_FALSE((*it)->at(129));
  EXPECT_FALSE(model.at(129));
}

} // namespace modbus_test
} // namespace asps_test
