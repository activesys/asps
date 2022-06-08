// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus config.

#include <gtest/gtest.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test modbus config
TEST(config_test, quantity_of_coils)
{
  EXPECT_EQ(config::quantity_of_coils(), config::max_quantity_of_coils);
  config::quantity_of_coils(0x0d);
  EXPECT_EQ(config::quantity_of_coils(), 0x0d);
  config::quantity_of_coils(0x0d0d);
  EXPECT_EQ(config::quantity_of_coils(), config::max_quantity_of_coils);
}

TEST(config_test, quantity_of_discrete_inputs)
{
  EXPECT_EQ(
    config::quantity_of_discrete_inputs(),
    config::max_quantity_of_discrete_inputs);
  config::quantity_of_discrete_inputs(0x0d);
  EXPECT_EQ(config::quantity_of_discrete_inputs(), 0x0d);
  config::quantity_of_discrete_inputs(0x0d0d);
  EXPECT_EQ(
    config::quantity_of_discrete_inputs(),
    config::max_quantity_of_discrete_inputs);
}

TEST(config_test, quantity_of_holding_registers)
{
  EXPECT_EQ(
    config::quantity_of_holding_registers(),
    config::max_quantity_of_holding_registers);
  config::quantity_of_holding_registers(0x0d);
  EXPECT_EQ(config::quantity_of_holding_registers(), 0x0d);
  config::quantity_of_holding_registers(0x0d0d);
  EXPECT_EQ(
    config::quantity_of_holding_registers(),
    config::max_quantity_of_holding_registers);
}

TEST(config_test, quantity_of_input_registers)
{
  EXPECT_EQ(
    config::quantity_of_input_registers(),
    config::max_quantity_of_input_registers);
  config::quantity_of_input_registers(0x0d);
  EXPECT_EQ(config::quantity_of_input_registers(), 0x0d);
  config::quantity_of_input_registers(0x0d0d);
  EXPECT_EQ(
    config::quantity_of_input_registers(),
    config::max_quantity_of_input_registers);
}

TEST(config_test, quantity_of_write_coils)
{
  EXPECT_EQ(
    config::quantity_of_write_coils(),
    config::max_quantity_of_write_coils);
  config::quantity_of_write_coils(0x0d);
  EXPECT_EQ(config::quantity_of_write_coils(), 0x0d);
  config::quantity_of_write_coils(0x0d0d);
  EXPECT_EQ(
    config::quantity_of_write_coils(),
    config::max_quantity_of_write_coils);
}

TEST(config_test, quantity_of_write_registers)
{
  EXPECT_EQ(
    config::quantity_of_write_registers(),
    config::max_quantity_of_write_registers);
  config::quantity_of_write_registers(0x0d);
  EXPECT_EQ(config::quantity_of_write_registers(), 0x0d);
  config::quantity_of_write_registers(0x0d0d);
  EXPECT_EQ(
    config::quantity_of_write_registers(),
    config::max_quantity_of_write_registers);
}

TEST(config_test, quantity_of_concurrent_requests)
{
  EXPECT_EQ(
    config::quantity_of_concurrent_requests(), 1);
  config::quantity_of_concurrent_requests(0x0d);
  EXPECT_EQ(config::quantity_of_concurrent_requests(), 0x0d);
  config::quantity_of_concurrent_requests(
    config::max_quantity_of_concurrent_requests);
  EXPECT_EQ(
    config::quantity_of_concurrent_requests(),
    config::max_quantity_of_concurrent_requests);
}

} // namespace modbus_test
} // namespace asps_test
