// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Client Sequence.

#include <gtest/gtest.h>
#include <asps/modbus/adu/sequence/client_sequence.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;

TEST(client_sequence_test, request_response)
{
  class active_observer_test : public adu::active_observer
  {
  public:
    virtual void update_pdu(const buffer_type& pdu) override
    {
      buffer_type expect_pdu_rsp{0x01, 0x02, 0xff, 0x03};
      EXPECT_EQ(pdu, expect_pdu_rsp);
    }
    virtual void update_event() override
    {}
  };
  buffer_type expect_pdu_req{0x01, 0x00, 0x64, 0x00, 0x0a};
  buffer_type expect_adu_req{0x07, 0x58, 0x00, 0x00, 0x00, 0x06, 0x05,
                             0x01, 0x00, 0x64, 0x00, 0x0a};
  buffer_type expect_adu_rsp{0x07, 0x58, 0x00, 0x00, 0x00, 0x05, 0x05,
                             0x01, 0x02, 0xff, 0x03};
  adu::client_sequence seq(1880, 5, expect_pdu_req);
  active_observer_test o;
  seq.register_event_observer(&o);
  EXPECT_EQ(seq.send_request(), expect_adu_req);
  EXPECT_TRUE(seq.receive_response(expect_adu_rsp));
}

} // modbus_test
} // asps_test
