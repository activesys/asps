// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus Session.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include <asps/modbus/pdu/semantic/request.hpp>
#include <asps/modbus/pdu/session/sequence_service.hpp>
#include <asps/modbus/pdu/session/session.hpp>

namespace asps_test {
namespace modbus_test {

using ::testing::_;
using ::testing::Return;
using namespace asps::modbus;

// TEST client session
TEST(client_session_test, request_response_once)
{
  class client_observer_test : public pdu::client_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x01, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::coils& status = dynamic_cast<const pdu::coils&>(datas);
      EXPECT_EQ(status.size(), 100);
      for (std::size_t i = 0; i < 100; i++) {
        EXPECT_EQ(status[i].address, i+100);
        EXPECT_TRUE(status[i].bit);
      }
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {}
  };
  pdu::client_session s;
  client_observer_test o;
  s.register_observer(&o);
  pdu::request::pointer_type r = pdu::make_read_coils_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x01, 0x0d, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0x0f};
  s.receive_response(expect_rsp);
}

TEST(client_session_test, request_response_multiple_times)
{
  class client_observer_test : public pdu::client_observer
  {
  public:
    client_observer_test()
      : send_count(0),
        receive_count(0)
    {}

  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      send_count++;
      if (send_count == 1) {
        pdu::buffer_type expect_pdu{0x01, 0x00, 0x64, 0x07, 0xd0};
        EXPECT_EQ(pdu, expect_pdu);
      } else {
        pdu::buffer_type expect_pdu{0x01, 0x08, 0x34, 0x03, 0xe8};
        EXPECT_EQ(pdu, expect_pdu);
      }
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      receive_count++;
      if (receive_count == 1) {
        const pdu::coils& status = dynamic_cast<const pdu::coils&>(datas);
        EXPECT_EQ(status.size(), 2000);
        for (std::size_t i = 0; i < 2000; i++) {
          EXPECT_EQ(status[i].address, i+100);
          EXPECT_TRUE(status[i].bit);
        }
      } else {
        const pdu::coils& status = dynamic_cast<const pdu::coils&>(datas);
        EXPECT_EQ(status.size(), 1000);
        for (std::size_t i = 0; i < 1000; i++) {
          EXPECT_EQ(status[i].address, i+2100);
          EXPECT_TRUE(status[i].bit);
        }
      }
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {}

  private:
    int send_count;
    int receive_count;
  };
  pdu::client_session s;
  client_observer_test o;
  s.register_observer(&o);
  pdu::request::pointer_type r = pdu::make_read_coils_request(100, 3000);

  s.send_request(r);

  pdu::buffer_type expect_rsp2000{0x01, 0xfa,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff};
  s.receive_response(expect_rsp2000);

  pdu::buffer_type expect_rsp1000{0x01, 0x7d,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff};
  s.receive_response(expect_rsp1000);
}

TEST(client_session_test, request_exception)
{
  class client_observer_test : public pdu::client_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x01, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::coils& status = dynamic_cast<const pdu::coils&>(datas);
      EXPECT_EQ(status.size(), 100);
      for (std::size_t i = 0; i < 100; i++) {
        EXPECT_EQ(status[i].address, i+100);
        EXPECT_TRUE(status[i].bit);
      }
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {
      const pdu::read_coils_request* r =
        dynamic_cast<const pdu::read_coils_request*>(req.get());
      if (r) {
        EXPECT_EQ(req->func_code(), pdu::function_code_read_coils);
        EXPECT_EQ(r->address(), 100);
        EXPECT_EQ(r->quantity(), 100);
        EXPECT_EQ(ec, pdu::exception_code_service_device_busy);
      } else {
        EXPECT_TRUE(false);
      }
    }
  };
  pdu::client_session s;
  client_observer_test o;
  s.register_observer(&o);
  pdu::request::pointer_type r = pdu::make_read_coils_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x81, 0x06};
  s.receive_response(expect_rsp);
}

} // modbus_test
} // asps_test
