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

} // modbus_test
} // asps_test
