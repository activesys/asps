// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Session.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include <asps/modbus/pdu/semantic/request.hpp>
#include <asps/modbus/pdu/sequence/sequence_service.hpp>
#include <asps/modbus/pdu/session/client_session.hpp>

namespace asps_test {
namespace modbus_test {

using ::testing::_;
using ::testing::Return;
using namespace asps::modbus;

// TEST client session
// Read Coils
TEST(client_session_test, read_coils_request_response_once)
{
  class client_observer_test : public pdu::client_session_observer
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

TEST(client_session_test, read_coils_request_response_multiple_times)
{
  class client_observer_test : public pdu::client_session_observer
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

TEST(client_session_test, read_coils_request_exception)
{
  class client_observer_test : public pdu::client_session_observer
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

// Read Discrete Inputs
TEST(client_session_test, read_discrete_inputs_request_response_once)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x02, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::discrete_inputs& status =
        dynamic_cast<const pdu::discrete_inputs&>(datas);
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
  pdu::request::pointer_type r = pdu::make_read_discrete_inputs_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x02, 0x0d, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0x0f};
  s.receive_response(expect_rsp);
}

TEST(client_session_test, read_discrete_inputs_request_response_multiple_times)
{
  class client_observer_test : public pdu::client_session_observer
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
        pdu::buffer_type expect_pdu{0x02, 0x00, 0x64, 0x07, 0xd0};
        EXPECT_EQ(pdu, expect_pdu);
      } else {
        pdu::buffer_type expect_pdu{0x02, 0x08, 0x34, 0x03, 0xe8};
        EXPECT_EQ(pdu, expect_pdu);
      }
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      receive_count++;
      if (receive_count == 1) {
        const pdu::discrete_inputs& status =
          dynamic_cast<const pdu::discrete_inputs&>(datas);
        EXPECT_EQ(status.size(), 2000);
        for (std::size_t i = 0; i < 2000; i++) {
          EXPECT_EQ(status[i].address, i+100);
          EXPECT_TRUE(status[i].bit);
        }
      } else {
        const pdu::discrete_inputs& status =
          dynamic_cast<const pdu::discrete_inputs&>(datas);
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
  pdu::request::pointer_type r = pdu::make_read_discrete_inputs_request(100, 3000);

  s.send_request(r);

  pdu::buffer_type expect_rsp2000{0x02, 0xfa,
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

  pdu::buffer_type expect_rsp1000{0x02, 0x7d,
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

TEST(client_session_test, read_discrete_inputs_request_exception)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x02, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::discrete_inputs& status =
        dynamic_cast<const pdu::discrete_inputs&>(datas);
      EXPECT_EQ(status.size(), 100);
      for (std::size_t i = 0; i < 100; i++) {
        EXPECT_EQ(status[i].address, i+100);
        EXPECT_TRUE(status[i].bit);
      }
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {
      const pdu::read_discrete_inputs_request* r =
        dynamic_cast<const pdu::read_discrete_inputs_request*>(req.get());
      if (r) {
        EXPECT_EQ(req->func_code(), pdu::function_code_read_discrete_inputs);
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
  pdu::request::pointer_type r = pdu::make_read_discrete_inputs_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x82, 0x06};
  s.receive_response(expect_rsp);
}

// Read Holding Registers
TEST(client_session_test, read_holding_registers_request_response_once)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x03, 0x00, 0x64, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::holding_registers& registers =
        dynamic_cast<const pdu::holding_registers&>(datas);
      pdu::holding_registers expect_registers{{100, 1234}, {101, 0}, {102, -9876},
                                              {103, 103}, {104, 104}, {105, 105},
                                              {106, 1060}, {107, 777}, {108, 888},
                                              {109, 9999}};
      EXPECT_EQ(registers, expect_registers);
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {}
  };
  pdu::client_session s;
  client_observer_test o;
  s.register_observer(&o);
  pdu::request::pointer_type r = pdu::make_read_holding_registers_request(100, 10);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x03, 0x14,
                              0x04, 0xd2, 0x00, 0x00, 0xd9, 0x6c,
                              0x00, 0x67, 0x00, 0x68, 0x00, 0x69,
                              0x04, 0x24, 0x03, 0x09, 0x03, 0x78,
                              0x27, 0x0f};
  s.receive_response(expect_rsp);
}

TEST(client_session_test, read_holding_registers_request_response_multiple_times)
{
  class client_observer_test : public pdu::client_session_observer
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
        pdu::buffer_type expect_pdu{0x03, 0x00, 0x64, 0x00, 0x7d};
        EXPECT_EQ(pdu, expect_pdu);
      } else {
        pdu::buffer_type expect_pdu{0x03, 0x00, 0xe1, 0x00, 0x4b};
        EXPECT_EQ(pdu, expect_pdu);
      }
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      receive_count++;
      if (receive_count == 1) {
        const pdu::holding_registers& registers =
          dynamic_cast<const pdu::holding_registers&>(datas);
        EXPECT_EQ(registers.size(), 125);
        for (std::size_t i = 0; i < 125; i++) {
          EXPECT_EQ(registers[i].address, i+100);
          EXPECT_EQ(registers[i].word, -1);
        }
      } else {
        const pdu::holding_registers& registers =
          dynamic_cast<const pdu::holding_registers&>(datas);
        EXPECT_EQ(registers.size(), 75);
        for (std::size_t i = 0; i < 75; i++) {
          EXPECT_EQ(registers[i].address, i+225);
          EXPECT_EQ(registers[i].word, -1);
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
  pdu::request::pointer_type r = pdu::make_read_holding_registers_request(100, 200);

  s.send_request(r);

  pdu::buffer_type expect_rsp125{0x03, 0xfa,
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
  s.receive_response(expect_rsp125);

  pdu::buffer_type expect_rsp75{0x03, 0x96,
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
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  s.receive_response(expect_rsp75);
}

TEST(client_session_test, read_holding_registers_request_exception)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x03, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {
      const pdu::read_holding_registers_request* r =
        dynamic_cast<const pdu::read_holding_registers_request*>(req.get());
      if (r) {
        EXPECT_EQ(req->func_code(), pdu::function_code_read_holding_registers);
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
  pdu::request::pointer_type r = pdu::make_read_holding_registers_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x83, 0x06};
  s.receive_response(expect_rsp);
}

// Read Holding Registers
TEST(client_session_test, read_input_registers_request_response_once)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x04, 0x00, 0x64, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      const pdu::input_registers& registers =
        dynamic_cast<const pdu::input_registers&>(datas);
      pdu::input_registers expect_registers{{100, 1234}, {101, 0}, {102, -9876},
                                              {103, 103}, {104, 104}, {105, 105},
                                              {106, 1060}, {107, 777}, {108, 888},
                                              {109, 9999}};
      EXPECT_EQ(registers, expect_registers);
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {}
  };
  pdu::client_session s;
  client_observer_test o;
  s.register_observer(&o);
  pdu::request::pointer_type r = pdu::make_read_input_registers_request(100, 10);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x04, 0x14,
                              0x04, 0xd2, 0x00, 0x00, 0xd9, 0x6c,
                              0x00, 0x67, 0x00, 0x68, 0x00, 0x69,
                              0x04, 0x24, 0x03, 0x09, 0x03, 0x78,
                              0x27, 0x0f};
  s.receive_response(expect_rsp);
}

TEST(client_session_test, read_input_registers_request_response_multiple_times)
{
  class client_observer_test : public pdu::client_session_observer
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
        pdu::buffer_type expect_pdu{0x04, 0x00, 0x64, 0x00, 0x7d};
        EXPECT_EQ(pdu, expect_pdu);
      } else {
        pdu::buffer_type expect_pdu{0x04, 0x00, 0xe1, 0x00, 0x4b};
        EXPECT_EQ(pdu, expect_pdu);
      }
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
      receive_count++;
      if (receive_count == 1) {
        const pdu::input_registers& registers =
          dynamic_cast<const pdu::input_registers&>(datas);
        EXPECT_EQ(registers.size(), 125);
        for (std::size_t i = 0; i < 125; i++) {
          EXPECT_EQ(registers[i].address, i+100);
          EXPECT_EQ(registers[i].word, -1);
        }
      } else {
        const pdu::input_registers& registers =
          dynamic_cast<const pdu::input_registers&>(datas);
        EXPECT_EQ(registers.size(), 75);
        for (std::size_t i = 0; i < 75; i++) {
          EXPECT_EQ(registers[i].address, i+225);
          EXPECT_EQ(registers[i].word, -1);
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
  pdu::request::pointer_type r = pdu::make_read_input_registers_request(100, 200);

  s.send_request(r);

  pdu::buffer_type expect_rsp125{0x03, 0xfa,
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
  s.receive_response(expect_rsp125);

  pdu::buffer_type expect_rsp75{0x03, 0x96,
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
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  s.receive_response(expect_rsp75);
}

TEST(client_session_test, read_input_registers_request_exception)
{
  class client_observer_test : public pdu::client_session_observer
  {
  public:
    virtual void update_send(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x04, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
    virtual void update_datas(const pdu::request::pointer_type& req,
                              const pdu::mb_datas& datas) override
    {
    }
    virtual void update_exception(const pdu::request::pointer_type& req,
                                  pdu::exception_code ec) override
    {
      const pdu::read_input_registers_request* r =
        dynamic_cast<const pdu::read_input_registers_request*>(req.get());
      if (r) {
        EXPECT_EQ(req->func_code(), pdu::function_code_read_input_registers);
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
  pdu::request::pointer_type r = pdu::make_read_input_registers_request(100, 100);

  s.send_request(r);
  pdu::buffer_type expect_rsp{0x84, 0x06};
  s.receive_response(expect_rsp);
}

} // modbus_test
} // asps_test
