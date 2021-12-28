// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Holding Registers and Input Registers.

#include <gtest/gtest.h>
#include <asps/modbus/registers.h>

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

TEST(holding_registers_test, operator_const_exception)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_THROW(hrs[0], std::out_of_range);
  EXPECT_THROW(hrs[110], std::out_of_range);
  EXPECT_NO_THROW(hrs[107]);
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

TEST(holding_registers_test, operator_assign_value_exception)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs(100, 10, registers);

  EXPECT_THROW({hrs[99] = 0;}, std::out_of_range);
  EXPECT_THROW({hrs[110] = 0;}, std::out_of_range);
  EXPECT_NO_THROW({hrs[104] = 87;});
}

TEST(holding_registers_test, operator_nullptr_exception)
{
  uint16_t registers[10] = {23, 98, 11, 0, 65535};
  holding_registers hrs1(100, 0, registers);

  EXPECT_THROW(hrs1[0], std::out_of_range);
  EXPECT_THROW({hrs1[0] = 0;}, std::out_of_range);

  holding_registers hrs2(100, 10, 0);
  EXPECT_THROW(hrs2[0], std::out_of_range);
  EXPECT_THROW({hrs2[0] = 199;}, std::out_of_range);
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

TEST(input_registers_test, operator_const_exception)
{
  uint16_t registers[10] = {77, 23, 1928, 444};
  input_registers irs(100, 10, registers);

  EXPECT_THROW(irs[4], std::out_of_range);
  EXPECT_THROW(irs[23445], std::out_of_range);
  EXPECT_NO_THROW(irs[109]);
}

TEST(input_registers_test, operator_const_nullptr_exception)
{
  uint16_t registers[10] = {77, 23, 1928, 444};
  input_registers irs1(100, 0, registers);

  EXPECT_THROW(irs1[0], std::out_of_range);

  input_registers irs2(100, 10, 0);
  EXPECT_THROW(irs2[0], std::out_of_range);
}

} // namespace modbus_test
} // namespace asps_test
