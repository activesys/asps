// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Request.

#include <gtest/gtest.h>
#include <asps/modbus/pdu/semantic/request.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

TEST(read_coils_request_test, invalid)
{
  pdu::read_coils_request req(100, 0);
  EXPECT_FALSE(req.valid());
}

TEST(read_coils_request_test, one_request)
{
  pdu::read_coils_request req(0, 100);
  EXPECT_TRUE(req.valid());
  EXPECT_EQ(req.address(), 0);
  EXPECT_EQ(req.quantity(), 100);
  pdu::request::pointer_type r = req.split();
  pdu::read_coils_request* p =
    dynamic_cast<pdu::read_coils_request*>(r.get());
  if (r && p) {
    EXPECT_TRUE(r->valid());
    EXPECT_EQ(p->address(), 0);
    EXPECT_EQ(p->quantity(), 100);
    EXPECT_FALSE(req.valid());
  } else {
    EXPECT_TRUE(false);
  }
}

TEST(read_coils_request_test, multiple_request)
{
  pdu::read_coils_request req(100, 5000);
  EXPECT_TRUE(req.valid());
  EXPECT_EQ(req.address(), 100);
  EXPECT_EQ(req.quantity(), 5000);
  pdu::read_coils_request* r =
    dynamic_cast<pdu::read_coils_request*>(req.split().get());
  if (r) {
    EXPECT_EQ(r->address(), 100);
    EXPECT_EQ(r->quantity(), 2000);
    EXPECT_TRUE(req.valid());
    EXPECT_EQ(req.address(), 2100);
    EXPECT_EQ(req.quantity(), 3000);
  } else {
    EXPECT_TRUE(false);
  }
}

} // modbus_test
} // asps_test

