// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus TCP Client.

#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <asps/utility/boost_env.hpp>
#include <asps/modbus/api/tcp_client.hpp>
#include <test/modbus/utility/modbus_test_server.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;
using namespace boost::asio;

class async_tcp_client_test : public ::testing::Test
{
protected:
  async_tcp_client_test()
    : server_("127.0.0.1", 9502)
  {
    g_context = std::make_shared<io_context>();
  }

  modbus_test_server server_;
};

TEST_F(async_tcp_client_test, connect_and_close)
{
  class my_async_tcp_client : public async_tcp_client
  {
  public:
    my_async_tcp_client(const std::string& ip, uint16_t port)
      : async_tcp_client(ip, port)
    {}

  public:
    virtual void on_connect() override
    {
      close();
      stop();
    }
  };

  server_.start(false);
  my_async_tcp_client c("127.0.0.1", 9502);
  c.connect(30);
  c.run();
  server_.wait();
}

TEST_F(async_tcp_client_test, read_coils)
{
  class my_async_tcp_client : public async_tcp_client
  {
  public:
    my_async_tcp_client(const std::string& ip, uint16_t port)
      : async_tcp_client(ip, port)
    {}

  public:
    virtual void on_connect() override
    {
      read_coils(10, 10);
    }
    virtual void on_read_coils(const pdu::coils& status) override
    {
      pdu::coils expect_status{{10,true},{11,true},{12,true},{13,true},
                               {14,true},{15,true},{16,true},{17,true},
                               {18,true},{19,true}};
      EXPECT_EQ(status, expect_status);
    }
  };

  buffer_type expect_rsp{0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01,
                         0x01, 0x02, 0xff, 0x03};
  server_.write_data(expect_rsp);
  server_.start();
  my_async_tcp_client c("127.0.0.1", 9502);
  c.connect(30);
  c.run();
  server_.wait();
}

TEST_F(async_tcp_client_test, read_discrete_inputs)
{
  class my_async_tcp_client : public async_tcp_client
  {
  public:
    my_async_tcp_client(const std::string& ip, uint16_t port)
      : async_tcp_client(ip, port)
    {}

  public:
    virtual void on_connect() override
    {
      read_discrete_inputs(10, 10);
    }
    virtual void on_read_discrete_inputs(const pdu::discrete_inputs& status) override
    {
      pdu::discrete_inputs expect_status{{10,true},{11,true},{12,true},{13,true},
                               {14,true},{15,true},{16,true},{17,true},
                               {18,true},{19,true}};
      EXPECT_EQ(status, expect_status);
    }
  };

  buffer_type expect_rsp{0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01,
                         0x02, 0x02, 0xff, 0x03};
  server_.write_data(expect_rsp);
  server_.start();
  my_async_tcp_client c("127.0.0.1", 9502);
  c.connect(30);
  c.run();
  server_.wait();
}

} // modbus_test
} // asps_test
