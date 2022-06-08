// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Client PDU Sequence.

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <asps/modbus/pdu/semantic/data_model.hpp>
#include <asps/modbus/pdu/sequence/client_sequence.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;

TEST(client_read_coils_sequence_test, request_response)
{
  class active_observer_test
    : public pdu::active_observer
  {
  public:
    virtual void update_event() override {}
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      pdu::coils expect_status{{0,false},{1,false},{2,false},{3,false},
                              {4,true},{5,true},{6,true},{7,true},
                              {8,true},{9,true}};
      EXPECT_EQ(req->func_code(), pdu::function_code_read_coils);
      const pdu::coils& status = dynamic_cast<const pdu::coils&>(datas);
      EXPECT_EQ(status, expect_status);
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {}
  };

  buffer_type expect_req{0x01, 0x00, 0x00, 0x00, 0x0a};
  buffer_type expect_rsp{0x01, 0x02, 0xf0, 0x03};
  active_observer_test o;
  pdu::client_read_coils_sequence s(pdu::make_read_coils_request(0, 10));
  s.register_event_observer(&o);
  EXPECT_EQ(expect_req, s.send_request());
  EXPECT_TRUE(s.receive_response(expect_rsp));
}

TEST(client_read_coils_sequence_test, request_exception)
{
  class active_observer_test
    : public pdu::active_observer
  {
  public:
    virtual void update_event() override {}
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {}
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {
      EXPECT_EQ(req->func_code(), pdu::function_code_read_coils);
      EXPECT_EQ(ec, pdu::exception_code_illegal_data_address);
      const pdu::read_coils_request* r =
        dynamic_cast<const pdu::read_coils_request*>(req.get());
      if (r) {
        EXPECT_EQ(r->address(), 0);
        EXPECT_EQ(r->quantity(), 10);
      } else {
        EXPECT_TRUE(false);
      }
    }
  };

  buffer_type expect_req{0x01, 0x00, 0x00, 0x00, 0x0a};
  buffer_type expect_excep{0x81, 0x02};
  active_observer_test o;
  pdu::client_read_coils_sequence s(pdu::make_read_coils_request(0, 10));
  s.register_event_observer(&o);
  EXPECT_EQ(expect_req, s.send_request());
  EXPECT_TRUE(s.receive_response(expect_excep));
}

} // modbus_test
} // asps_test
