// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Message.

#include <gtest/gtest.h>
#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/message/client_message.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;

// Test PDU unserialize
// Read Coils
TEST(client_read_coils_request_test, serialize)
{
  buffer_type msg1{0x01, 0x00, 0x55, 0x00, 0x86};
  pdu::client_read_coils_request r1(85, 134);
  const buffer_type& b1 = r1.serialize();
  EXPECT_EQ(b1, msg1);

  buffer_type msg2{0x01, 0x1a, 0x28, 0x07, 0x20};
  pdu::client_read_coils_request r2(6696, 1824);
  const buffer_type& b2 = r2.serialize();
  EXPECT_EQ(b2, msg2);
}

TEST(client_read_coils_response_test, unserialize)
{
  pdu::coils status{{0,false},
                    {0,true},
                    {0,true},
                    {0,false},
                    {0,true},
                    {0,false},
                    {0,false},
                    {0,false}};
  buffer_type msg{0x01, 0x01, 0x16};
  pdu::message_unserialization_service::pointer_type r = pdu::make_client_read_coils_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::coils& s = dynamic_cast<pdu::coils&>(r->datas());
  EXPECT_EQ(status, s);
}

TEST(client_read_coils_response_test, unserialize_multiple_bytes)
{
  pdu::coils status{{0,false},{0,true},{0,true},{0,false},
                    {0,true},{0,false},{0,false},{0,false},
                    {0,true},{0,false},{0,false},{0,false},
                    {0,true},{0,false},{0,false},{0,false}};
  buffer_type msg{0x01, 0x02, 0x16, 0x11};
  pdu::message_unserialization_service::pointer_type r = pdu::make_client_read_coils_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::coils& s = dynamic_cast<pdu::coils&>(r->datas());
  EXPECT_EQ(status, s);
}

TEST(client_read_coils_response_test, unserialize_exception)
{
  buffer_type msg{0x81, 0x02};
  pdu::message_unserialization_service::pointer_type r = pdu::make_client_read_coils_response();
  EXPECT_TRUE(r->unserialize(msg));
  EXPECT_EQ(r->func_code(), pdu::function_code_read_coils);
  EXPECT_EQ(r->excep_code(), pdu::exception_code_illegal_data_address);
}

// Read Discrete Inputs
TEST(client_read_discrete_inputs_request_test, serialize)
{
  buffer_type msg1{0x02, 0x00, 0x55, 0x00, 0x86};
  pdu::client_read_discrete_inputs_request r1(85, 134);
  const buffer_type& b1 = r1.serialize();
  EXPECT_EQ(b1, msg1);

  buffer_type msg2{0x02, 0x1a, 0x28, 0x07, 0x20};
  pdu::client_read_discrete_inputs_request r2(6696, 1824);
  const buffer_type& b2 = r2.serialize();
  EXPECT_EQ(b2, msg2);
}

TEST(client_read_discrete_inputs_response_test, unserialize)
{
  pdu::discrete_inputs status{{0,false}, {0,true}, {0,true}, {0,false},
                              {0,true}, {0,false}, {0,false}, {0,false}};
  buffer_type msg{0x02, 0x01, 0x16};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_discrete_inputs_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::discrete_inputs& s = dynamic_cast<pdu::discrete_inputs&>(r->datas());
  EXPECT_EQ(status, s);
}

TEST(client_read_discrete_inputs_response_test, unserialize_multiple_bytes)
{
  pdu::discrete_inputs status{{0,false},{0,true},{0,true},{0,false},
                              {0,true},{0,false},{0,false},{0,false},
                              {0,true},{0,false},{0,false},{0,false},
                              {0,true},{0,false},{0,false},{0,false}};
  buffer_type msg{0x02, 0x02, 0x16, 0x11};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_discrete_inputs_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::discrete_inputs& s = dynamic_cast<pdu::discrete_inputs&>(r->datas());
  EXPECT_EQ(status, s);
}

TEST(client_read_discrete_inputs_response_test, unserialize_exception)
{
  buffer_type msg{0x82, 0x05};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_discrete_inputs_response();
  EXPECT_TRUE(r->unserialize(msg));
  EXPECT_EQ(r->func_code(), pdu::function_code_read_discrete_inputs);
  EXPECT_EQ(r->excep_code(), pdu::exception_code_acknowledge);
}

// Read Holding Registers
TEST(client_read_holding_registers_request_test, serialize)
{
  buffer_type msg1{0x03, 0x00, 0x55, 0x00, 0x86};
  pdu::client_read_holding_registers_request r1(85, 134);
  const buffer_type& b1 = r1.serialize();
  EXPECT_EQ(b1, msg1);

  buffer_type msg2{0x03, 0x1a, 0x28, 0x00, 0x7d};
  pdu::client_read_holding_registers_request r2(6696, 125);
  const buffer_type& b2 = r2.serialize();
  EXPECT_EQ(b2, msg2);
}

TEST(client_read_holding_registers_response_test, unserialize)
{
  pdu::holding_registers registers{{0,26}, {0,72}, {0,-9}, {0,1234},
                                {0,690}, {0,-13}, {0,0}, {0,2322}};
  buffer_type msg{0x03, 0x10,
                  0x00, 0x1a, 0x00, 0x48, 0xff, 0xf7, 0x04, 0xd2,
                  0x02, 0xb2, 0xff, 0xf3, 0x00, 0x00, 0x09, 0x12};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_holding_registers_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::holding_registers& s = dynamic_cast<pdu::holding_registers&>(r->datas());
  EXPECT_EQ(registers, s);
}

TEST(client_read_holding_registers_response_test, unserialize_exception)
{
  buffer_type msg{0x83, 0x04};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_holding_registers_response();
  EXPECT_TRUE(r->unserialize(msg));
  EXPECT_EQ(r->func_code(), pdu::function_code_read_holding_registers);
  EXPECT_EQ(r->excep_code(), pdu::exception_code_server_device_failure);
}

// Read Input Registers
TEST(client_read_input_registers_request_test, serialize)
{
  buffer_type msg1{0x04, 0x00, 0x55, 0x00, 0x86};
  pdu::client_read_input_registers_request r1(85, 134);
  const buffer_type& b1 = r1.serialize();
  EXPECT_EQ(b1, msg1);

  buffer_type msg2{0x04, 0x1a, 0x28, 0x00, 0x7d};
  pdu::client_read_input_registers_request r2(6696, 125);
  const buffer_type& b2 = r2.serialize();
  EXPECT_EQ(b2, msg2);
}

TEST(client_read_input_registers_response_test, unserialize)
{
  pdu::input_registers registers{{0,26}, {0,72}, {0,-9}, {0,1234},
                                {0,690}, {0,-13}, {0,0}, {0,2322}};
  buffer_type msg{0x04, 0x10,
                  0x00, 0x1a, 0x00, 0x48, 0xff, 0xf7, 0x04, 0xd2,
                  0x02, 0xb2, 0xff, 0xf3, 0x00, 0x00, 0x09, 0x12};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_input_registers_response();
  EXPECT_TRUE(r->unserialize(msg));
  const pdu::input_registers& s = dynamic_cast<pdu::input_registers&>(r->datas());
  EXPECT_EQ(registers, s);
}

TEST(client_read_input_registers_response_test, unserialize_exception)
{
  buffer_type msg{0x84, 0x04};
  pdu::message_unserialization_service::pointer_type r =
    pdu::make_client_read_input_registers_response();
  EXPECT_TRUE(r->unserialize(msg));
  EXPECT_EQ(r->func_code(), pdu::function_code_read_input_registers);
  EXPECT_EQ(r->excep_code(), pdu::exception_code_server_device_failure);
}

} // modbus_test
} // asps_test
