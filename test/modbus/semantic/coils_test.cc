// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Coils and Discrete Inputs.

#include <deque>
#include <iterator>
#include <gtest/gtest.h>
#include <asps/modbus/semantic/coils.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test Coils
TEST(coils_test, bit_const_normal)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs.bit(200));
  EXPECT_TRUE(cs.bit(201));
  EXPECT_TRUE(cs.bit(202));
  EXPECT_FALSE(cs.bit(203));
  EXPECT_FALSE(cs.bit(204));
}

TEST(coils_test, bit_assign_status)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs.bit(200));
  EXPECT_TRUE(cs.bit(201));
  cs.bit(200, true);
  cs.bit(201, false);
  EXPECT_TRUE(cs.bit(200));
  EXPECT_FALSE(cs.bit(201));
}

TEST(coils_test, bit_const_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW(cs.bit(199), std::out_of_range);
  EXPECT_THROW(cs.bit(210), std::out_of_range);
  EXPECT_NO_THROW(cs.bit(201));
}

TEST(coils_test, bit_assign_status_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW({cs.bit(199, true);}, std::out_of_range);
  EXPECT_THROW({cs.bit(210, true);}, std::out_of_range);
  EXPECT_NO_THROW({cs.bit(208, false);});
}

TEST(coils_test, split)
{
  bool datas[30] = {
    false, true, true, false, false, false, true, true,
    true, true, true, true, true, true, false, true, 
    true, true, true, true, false, false, false, false, 
    false, true, true, false, false, true};
  coils cs(100, 30, datas);

  std::deque<coils::pointer_type> q;
  coils::split(cs, 8, std::back_inserter(q));
  EXPECT_EQ(q.size(), 4);
  std::deque<coils::pointer_type>::iterator it = q.begin();
  // the first 8 elements
  EXPECT_FALSE((*it)->bit(100));
  EXPECT_TRUE((*it)->bit(101));
  EXPECT_TRUE((*it)->bit(102));
  EXPECT_FALSE((*it)->bit(103));
  EXPECT_FALSE((*it)->bit(104));
  EXPECT_FALSE((*it)->bit(105));
  EXPECT_TRUE((*it)->bit(106));
  EXPECT_TRUE((*it)->bit(107));
  EXPECT_THROW((*it)->bit(99), std::out_of_range);
  EXPECT_THROW((*it)->bit(108), std::out_of_range);
  (*it)->bit(103, true);
  EXPECT_TRUE((*it)->bit(103));
  EXPECT_FALSE(cs.bit(103));
  ++it;
  // the secode 8 elements
  EXPECT_TRUE((*it)->bit(108));
  EXPECT_TRUE((*it)->bit(109));
  EXPECT_TRUE((*it)->bit(110));
  EXPECT_TRUE((*it)->bit(111));
  EXPECT_TRUE((*it)->bit(112));
  EXPECT_TRUE((*it)->bit(113));
  EXPECT_FALSE((*it)->bit(114));
  EXPECT_TRUE((*it)->bit(115));
  EXPECT_THROW((*it)->bit(107), std::out_of_range);
  EXPECT_THROW((*it)->bit(116), std::out_of_range);
  (*it)->bit(114, true);
  EXPECT_TRUE((*it)->bit(114));
  EXPECT_FALSE(cs.bit(114));
  ++it;
  // the third 8 elements
  EXPECT_TRUE((*it)->bit(116));
  EXPECT_TRUE((*it)->bit(117));
  EXPECT_TRUE((*it)->bit(118));
  EXPECT_TRUE((*it)->bit(119));
  EXPECT_FALSE((*it)->bit(120));
  EXPECT_FALSE((*it)->bit(121));
  EXPECT_FALSE((*it)->bit(122));
  EXPECT_FALSE((*it)->bit(123));
  EXPECT_THROW((*it)->bit(115), std::out_of_range);
  EXPECT_THROW((*it)->bit(124), std::out_of_range);
  (*it)->bit(118, false);
  EXPECT_FALSE((*it)->bit(118));
  EXPECT_TRUE(cs.bit(118));
  ++it;
  // the last 6 elements
  EXPECT_FALSE((*it)->bit(124));
  EXPECT_TRUE((*it)->bit(125));
  EXPECT_TRUE((*it)->bit(126));
  EXPECT_FALSE((*it)->bit(127));
  EXPECT_FALSE((*it)->bit(128));
  EXPECT_TRUE((*it)->bit(129));
  EXPECT_THROW((*it)->bit(123), std::out_of_range);
  EXPECT_THROW((*it)->bit(130), std::out_of_range);
  (*it)->bit(129, false);
  EXPECT_FALSE((*it)->bit(129));
  EXPECT_TRUE(cs.bit(129));
}

#if 0
// Test Discrete Inputs
TEST(discrete_inputs_test, operator_const_normal)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis(200, 10, status);

  EXPECT_FALSE(dis[200]);
  EXPECT_TRUE(dis[201]);
  EXPECT_TRUE(dis[202]);
  EXPECT_FALSE(dis[203]);
  EXPECT_FALSE(dis[204]);
}

TEST(discrete_inputs_test, at_const_normal)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis(200, 10, status);

  EXPECT_FALSE(dis.bit(200));
  EXPECT_TRUE(dis.bit(201));
  EXPECT_TRUE(dis.bit(202));
  EXPECT_FALSE(dis.bit(203));
  EXPECT_FALSE(dis.bit(204));
}

TEST(discrete_inputs_test, at_const_exception)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis(200, 10, status);

  EXPECT_THROW(dis.bit(199), std::out_of_range);
  EXPECT_THROW(dis.bit(210), std::out_of_range);
  EXPECT_NO_THROW(dis.bit(204));
}

TEST(discrete_inputs_test, at_nullptr_exception)
{
  discrete_inputs dis(200, 10, nullptr);

  EXPECT_THROW(dis.bit(204), std::out_of_range);
}
#endif

} // namespace modbus_test
} // namespace asps_test
