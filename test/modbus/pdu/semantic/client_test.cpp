// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Client.

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/pdu/semantic/pdu_client.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace std::placeholders;

// Read Coils
TEST(pdu_client_test, read_coils)
{
  class adu_test : public frame::adu_service
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

  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.read_coils(10, 100);
}

TEST(pdu_client_test, on_read_coils)
{
  class adu_test : public frame::adu_service
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
  class my_handler
  {
  public:
    void on_read_coils(const pdu::coils& status)
    {
      pdu::coils expect_status{{10,true},{11,true},{12,true},{13,true},
                               {14,true},{15,true},{16,true},{17,true},
                               {18,true},{19,true}};
      EXPECT_EQ(status, expect_status);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_read_coils_handler(std::bind(&my_handler::on_read_coils, &h, _1));
  c.read_coils(10, 10);
}

TEST(pdu_client_test, on_exception_read_coils)
{
  class adu_test : public frame::adu_service
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
  class my_handler
  {
  public:
    void on_exception(pdu::function_code fc, pdu::exception_code ec)
    {
      EXPECT_EQ(fc, pdu::function_code_read_coils);
      EXPECT_EQ(ec, pdu::exception_code_memory_parity_error);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_exception_handler(std::bind(&my_handler::on_exception, &h, _1, _2));
  c.read_coils(10, 10);
}

// Read Discrete Inputs
TEST(pdu_client_test, read_discrete_inputs)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {}
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x02, 0x00, 0x0a, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };

  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.read_discrete_inputs(10, 100);
}

TEST(pdu_client_test, on_read_discrete_inputs)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x02, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x02, 0x02, 0xff, 0x03};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_read_discrete_inputs(const pdu::discrete_inputs& status)
    {
      pdu::discrete_inputs expect_status{{10,true},{11,true},{12,true},{13,true},
                                         {14,true},{15,true},{16,true},{17,true},
                                         {18,true},{19,true}};
      EXPECT_EQ(status, expect_status);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_read_discrete_inputs_handler(std::bind(&my_handler::on_read_discrete_inputs, &h, _1));
  c.read_discrete_inputs(10, 10);
}

TEST(pdu_client_test, on_exception_read_discrete_inputs)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x02, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x82, 0x08};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_exception(pdu::function_code fc, pdu::exception_code ec)
    {
      EXPECT_EQ(fc, pdu::function_code_read_discrete_inputs);
      EXPECT_EQ(ec, pdu::exception_code_memory_parity_error);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_exception_handler(std::bind(&my_handler::on_exception, &h, _1, _2));
  c.read_discrete_inputs(10, 10);
}

// Read Holding registers
TEST(pdu_client_test, read_holding_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {}
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x03, 0x00, 0x0a, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };

  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.read_holding_registers(10, 100);
}

TEST(pdu_client_test, on_read_holding_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x03, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x03, 0x14,
                                  0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x0d,
                                  0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x11,
                                  0x00, 0x12, 0x00, 0x13};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_read_holding_registers(const pdu::holding_registers& registers)
    {
      pdu::holding_registers expect_registers{{10,10},{11,11},{12,12},{13,13},
                                         {14,14},{15,15},{16,16},{17,17},
                                         {18,18},{19,19}};
      EXPECT_EQ(registers, expect_registers);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_read_holding_registers_handler(std::bind(&my_handler::on_read_holding_registers, &h, _1));
  c.read_holding_registers(10, 10);
}

TEST(pdu_client_test, on_exception_read_holding_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x03, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x83, 0x08};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_exception(pdu::function_code fc, pdu::exception_code ec)
    {
      EXPECT_EQ(fc, pdu::function_code_read_holding_registers);
      EXPECT_EQ(ec, pdu::exception_code_memory_parity_error);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_exception_handler(std::bind(&my_handler::on_exception, &h, _1, _2));
  c.read_holding_registers(10, 10);
}

// Read Input registers
TEST(pdu_client_test, read_input_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {}
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x04, 0x00, 0x0a, 0x00, 0x64};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };

  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.read_input_registers(10, 100);
}

TEST(pdu_client_test, on_read_input_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x04, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x04, 0x14,
                                  0x00, 0x0a, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x0d,
                                  0x00, 0x0e, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x11,
                                  0x00, 0x12, 0x00, 0x13};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_read_input_registers(const pdu::input_registers& registers)
    {
      pdu::input_registers expect_registers{{10,10},{11,11},{12,12},{13,13},
                                         {14,14},{15,15},{16,16},{17,17},
                                         {18,18},{19,19}};
      EXPECT_EQ(registers, expect_registers);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_read_input_registers_handler(std::bind(&my_handler::on_read_input_registers, &h, _1));
  c.read_input_registers(10, 10);
}

TEST(pdu_client_test, on_exception_read_input_registers)
{
  class adu_test : public frame::adu_service
  {
  public:
    virtual void set_handler(read_handler handler) override
    {
      handler_ = handler;
    }
    virtual void write(const pdu::buffer_type& pdu) override
    {
      pdu::buffer_type expect_pdu{0x04, 0x00, 0x0a, 0x00, 0x0a};
      EXPECT_EQ(pdu, expect_pdu);

      pdu::buffer_type expect_rsp{0x84, 0x08};
      handler_(expect_rsp);
    }

  private:
    read_handler handler_;
  };
  class my_handler
  {
  public:
    void on_exception(pdu::function_code fc, pdu::exception_code ec)
    {
      EXPECT_EQ(fc, pdu::function_code_read_input_registers);
      EXPECT_EQ(ec, pdu::exception_code_memory_parity_error);
    }
  };

  my_handler h;
  frame::adu_service::pointer_type adu = std::make_shared<adu_test>();
  pdu::pdu_client c(adu);
  c.set_exception_handler(std::bind(&my_handler::on_exception, &h, _1, _2));
  c.read_input_registers(10, 10);
}

} // modbus_test
} // asps_test
