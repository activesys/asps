// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Holding Registers and Input Registers.

#include <gtest/gtest.h>
#include <asps/modbus/semantic/registers.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test Holding Registers
TEST(holding_registers_test, operator_const_normal)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_EQ(hrs[100], 23);
  EXPECT_EQ(hrs[101], 98);
  EXPECT_EQ(hrs[102], 11);
  EXPECT_EQ(hrs[103], 0);
  EXPECT_EQ(hrs[104], 65535);
  EXPECT_EQ(hrs[105], 0);
}

TEST(holding_registers_test, operator_assign_value)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_EQ(hrs[101], 98);
  EXPECT_EQ(hrs[103], 0);
  hrs[101] = 9999;
  hrs[103] = 6666;
  EXPECT_EQ(hrs[101], 9999);
  EXPECT_EQ(hrs[103], 6666);
}

TEST(holding_registers_test, at_const_normal)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_EQ(hrs.at(100), 23);
  EXPECT_EQ(hrs.at(101), 98);
  EXPECT_EQ(hrs.at(102), 11);
  EXPECT_EQ(hrs.at(103), 0);
  EXPECT_EQ(hrs.at(104), 65535);
  EXPECT_EQ(hrs.at(105), 0);
}

TEST(holding_registers_test, at_assign_value)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_EQ(hrs.at(101), 98);
  EXPECT_EQ(hrs.at(103), 0);
  hrs.at(101) = 9999;
  hrs.at(103) = 6666;
  EXPECT_EQ(hrs.at(101), 9999);
  EXPECT_EQ(hrs.at(103), 6666);
}

TEST(holding_registers_test, at_const_exception)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_THROW(hrs.at(0), std::out_of_range);
  EXPECT_THROW(hrs.at(110), std::out_of_range);
  EXPECT_NO_THROW(hrs.at(107));
}

TEST(holding_registers_test, at_assign_value_exception)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_THROW({hrs.at(99) = 0;}, std::out_of_range);
  EXPECT_THROW({hrs.at(110) = 0;}, std::out_of_range);
  EXPECT_NO_THROW({hrs.at(104) = 87;});
}

TEST(holding_registers_test, at_nullptr_exception)
{
  holding_registers hrs(100, 10, nullptr);

  EXPECT_THROW(hrs.at(104), std::out_of_range);
  EXPECT_THROW({hrs.at(104) = 0;}, std::out_of_range);
}

// Test Input Registers
TEST(input_registers_test, operator_const_normal)
{
  uint16_t registers[10] = {77, 23, 1928, 444};
  input_registers irs(100, 10, registers);

  EXPECT_EQ(irs[100], 77);
  EXPECT_EQ(irs[101], 23);
  EXPECT_EQ(irs[102], 1928);
  EXPECT_EQ(irs[103], 444);
  EXPECT_EQ(irs[104], 0);
}

TEST(input_registers_test, at_const_normal)
{
  uint16_t registers[10] = {77, 23, 1928, 444};
  input_registers irs(100, 10, registers);

  EXPECT_EQ(irs.at(100), 77);
  EXPECT_EQ(irs.at(101), 23);
  EXPECT_EQ(irs.at(102), 1928);
  EXPECT_EQ(irs.at(103), 444);
  EXPECT_EQ(irs.at(104), 0);
}

TEST(input_registers_test, at_const_exception)
{
  uint16_t registers[10] = {77, 23, 1928, 444};
  input_registers irs(100, 10, registers);

  EXPECT_THROW(irs.at(4), std::out_of_range);
  EXPECT_THROW(irs.at(23445), std::out_of_range);
  EXPECT_NO_THROW(irs.at(109));
}

TEST(input_registers_test, at_nullptr_exception)
{
  input_registers irs(100, 10, nullptr);

  EXPECT_THROW(irs.at(109), std::out_of_range);
}

} // namespace modbus_test
} // namespace asps_test
