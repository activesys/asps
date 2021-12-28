// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Coils and Discrete Inputs.

#include <gtest/gtest.h>
#include <asps/modbus/coils.h>

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

TEST(coils_test, operator_const_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW(cs[199], std::out_of_range);
  EXPECT_THROW(cs[210], std::out_of_range);
  EXPECT_NO_THROW(cs[201]);
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

TEST(coils_test, operator_assign_status_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs(200, 10, status);

  EXPECT_THROW({cs[199] = true;}, std::out_of_range);
  EXPECT_THROW({cs[210] = true;}, std::out_of_range);
  EXPECT_NO_THROW({cs[208] = false;});
}

TEST(coils_test, operator_nullptr_exception)
{
  bool status[10] = {false, true, true, false};
  coils cs1(200, 0, status);

  EXPECT_THROW(cs1[200], std::out_of_range);
  EXPECT_THROW({cs1[200] = true;}, std::out_of_range);

  coils cs2(200, 10, 0);
  EXPECT_THROW(cs2[200], std::out_of_range);
  EXPECT_THROW({cs2[200] = true;}, std::out_of_range);
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

TEST(discrete_inputs_test, operator_const_exception)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis(200, 10, status);

  EXPECT_THROW(dis[199], std::out_of_range);
  EXPECT_THROW(dis[210], std::out_of_range);
  EXPECT_NO_THROW(dis[204]);
}

TEST(discrete_inputs_test, operator_nullptr_exception)
{
  bool status[10] = {false, true, true, false};
  discrete_inputs dis1(200, 0, status);

  EXPECT_THROW(dis1[200], std::out_of_range);

  discrete_inputs dis2(200, 10, 0);
  EXPECT_THROW(dis2[200], std::out_of_range);
}

} // namespace modbus_test
} // namespace asps_test
