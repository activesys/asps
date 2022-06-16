// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Client Message.

#include <gtest/gtest.h>
#include <asps/modbus/adu/message/client_message.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;

// TEST tcp_adu
TEST(client_request_adu_test, serialize)
{
  buffer_type expect_pdu{0x01, 0x00, 0x01, 0x00, 0x64};
  buffer_type expcet_adu{0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x08,
                         0x01, 0x00, 0x01, 0x00, 0x64};
  adu::client_request_adu a(1,8,expect_pdu);
  EXPECT_EQ(a.serialize(), expcet_adu);
}

TEST(client_response_adu_test, unserialize)
{
  buffer_type expect_adu{0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01,
                         0x01, 0x01, 0xff};
  adu::client_response_adu a;
  EXPECT_TRUE(a.unserialize(expect_adu));
  buffer_type expect_pdu{0x01, 0x01, 0xff};
  EXPECT_EQ(a.pdu(), expect_pdu);
  EXPECT_EQ(a.tid(), 0);
  EXPECT_EQ(a.uid(), 1);
  EXPECT_EQ(a.pid(), 0);
}

} // modbus_test
} // asps_test
