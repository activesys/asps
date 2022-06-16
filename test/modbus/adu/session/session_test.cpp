// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Session.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/adu/session/session.hpp>

namespace asps_test {
namespace modbus_test {

using ::testing::_;
using ::testing::Return;
using namespace asps::modbus;
using namespace asps::utility;

// TEST client session
TEST(adu_client_session_test, request_response)
{
  class client_observer_test : public adu::client_observer
  {
  public:
    virtual void update_send(const buffer_type& adu) override
    {
      buffer_type expect_adu{0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01,
                             0x01, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(adu, expect_adu);
    }
    virtual void update_pdu(const buffer_type& pdu) override
    {
      buffer_type expect_pdu{0x01, 0x0d, 0xff, 0xff, 0xff, 0xff,
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             0xff, 0xff, 0x0f};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };
  adu::client_session s;
  client_observer_test o;
  s.register_observer(&o);

  buffer_type expect_pdu{0x01, 0x00, 0x64, 0x00, 0x64};
  s.send_request(expect_pdu);
  buffer_type expect_rsp{0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01,
                         0x01, 0x0d, 0xff, 0xff, 0xff, 0xff,
                         0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                         0xff, 0xff, 0x0f};
  s.receive_response(expect_rsp);
}

} // modbus_test
} // asps_test
