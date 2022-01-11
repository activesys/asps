// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Coils and Discrete Inputs.

#include <gtest/gtest.h>
#include <asps/modbus/semantic/coils.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test Coils
TEST(coils_test, operator_const_normal)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs[200]);
  EXPECT_TRUE(cs[201]);
  EXPECT_TRUE(cs[202]);
  EXPECT_FALSE(cs[203]);
  EXPECT_FALSE(cs[204]);
}

TEST(coils_test, operator_assign_status)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs[200]);
  EXPECT_TRUE(cs[201]);
  cs[200] = true;
  cs[201] = false;
  EXPECT_TRUE(cs[200]);
  EXPECT_FALSE(cs[201]);
}

TEST(coils_test, at_const_normal)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs[200]);
  EXPECT_TRUE(cs[201]);
  EXPECT_TRUE(cs[202]);
  EXPECT_FALSE(cs[203]);
  EXPECT_FALSE(cs[204]);
}

TEST(coils_test, at_assign_status)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_FALSE(cs.at(200));
  EXPECT_TRUE(cs.at(201));
  cs.at(200) = true;
  cs.at(201) = false;
  EXPECT_TRUE(cs.at(200));
  EXPECT_FALSE(cs.at(201));
}

TEST(coils_test, at_const_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW(cs.at(199), std::out_of_range);
  EXPECT_THROW(cs.at(210), std::out_of_range);
  EXPECT_NO_THROW(cs.at(201));
}

TEST(coils_test, at_assign_status_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW({cs.at(199) = true;}, std::out_of_range);
  EXPECT_THROW({cs.at(210) = true;}, std::out_of_range);
  EXPECT_NO_THROW({cs.at(208) = false;});
}

TEST(coils_test, at_nullptr_exception)
{
  coils cs(200, 10, nullptr);

  EXPECT_THROW(cs.at(208), std::out_of_range);
  EXPECT_THROW({cs.at(208) = true;}, std::out_of_range);
}

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

  EXPECT_FALSE(dis.at(200));
  EXPECT_TRUE(dis.at(201));
  EXPECT_TRUE(dis.at(202));
  EXPECT_FALSE(dis.at(203));
  EXPECT_FALSE(dis.at(204));
}

TEST(discrete_inputs_test, at_const_exception)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis(200, 10, status);

  EXPECT_THROW(dis.at(199), std::out_of_range);
  EXPECT_THROW(dis.at(210), std::out_of_range);
  EXPECT_NO_THROW(dis.at(204));
}

TEST(discrete_inputs_test, at_nullptr_exception)
{
  discrete_inputs dis(200, 10, nullptr);

  EXPECT_THROW(dis.at(204), std::out_of_range);
}

} // namespace modbus_test
} // namespace asps_test
