// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Client.

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/pdu/semantic/client.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

TEST(pdu_client_test, read_coils)
{
  class adu_test : public pdu::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {}
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x01, 0x00, 0x0a, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };

  pdu::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::client c(adu);
  c.read_coils(10, 100);
}

TEST(pdu_client_test, on_read_coils)
{
  class adu_test : public pdu::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x01, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x01, 0x02, 0xff, 0x03};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_client : public pdu::client
  {
  public:
    my_client(pdu::adu_service::pointer_type adu)
      : pdu::client(adu)
    {}
  public:
    virtual void on_read_coils(const pdu::coils& status) override
    {
      pdu::coils expect_status{{10,true},{11,true},{12,true},{13,true},
                               {14,true},{15,true},{16,true},{17,true},
                               {18,true},{19,true}};
      EXPECT_EQ(status, expect_status);
    }
    virtual void on_exception(pdu::function_code fc, pdu::exception_code ec) override
    {}
  };

  pdu::adu_service::pointer_type adu = std::make_shared<adu_test>();
  my_client c(adu);
  c.read_coils(10, 10);
}

TEST(pdu_client_test, on_exception)
{
  class adu_test : public pdu::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x01, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x81, 0x08};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_client : public pdu::client
  {
  public:
    my_client(pdu::adu_service::pointer_type adu)
      : pdu::client(adu)
    {}
  public:
    virtual void on_read_coils(const pdu::coils& status) override
    {}
    virtual void on_exception(pdu::function_code fc, pdu::exception_code ec) override
    {
      EXPECT_EQ(fc, pdu::function_code_read_coils);
      EXPECT_EQ(ec, pdu::exception_code_memory_parity_error);
    }
  };

  pdu::adu_service::pointer_type adu = std::make_shared<adu_test>();
  my_client c(adu);
  c.read_coils(10, 10);
}

} // modbus_test
} // asps_test
