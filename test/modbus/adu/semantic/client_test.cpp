// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Client.

#include <gtest/gtest.h>
#include <asps/modbus/adu/semantic/adu_client.hpp>
#include <asps/transport/transport_service.hpp>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using namespace asps::utility;
using namespace asps::transport;

TEST(adu_client_test, write)
{
  class connection_test : public connection
  {
  public:
    virtual void set_read_handler(const read_handler rhandler) override{}
    virtual void set_write_handler(const write_handler whandler) override {}
    virtual void set_close_handler(const close_handler chandler) override {}
    virtual void read() override
    {}
    virtual void write(const buffer_type& buf) override
    {
      buffer_type expect_adu{0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01,
                             0x01, 0x00, 0x64, 0x00, 0x64};
      EXPECT_EQ(buf, expect_adu);
    }
    virtual void close() override
    {}
    virtual std::string remote_address() override
    {
      return "127.0.0.1";
    }
    virtual uint16_t remote_port() override
    {
      return 502;
    }
  };
  buffer_type expect_pdu{0x01, 0x00, 0x64, 0x00, 0x64};
  connection::pointer_type conn = std::make_shared<connection_test>();
  adu::adu_client c(conn);
  c.write(expect_pdu);
}

TEST(adu_client_test, read)
{
  class connection_test : public connection, public std::enable_shared_from_this<connection>
  {
  public:
    virtual void set_read_handler(const read_handler rhandler) override
    {rhandler_ = rhandler;}
    virtual void set_write_handler(const write_handler whandler) override {}
    virtual void set_close_handler(const close_handler chandler) override {}
    virtual void read() override
    {
      buffer_type expect_adu{0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01,
                             0x81, 0x02};
      rhandler_(shared_from_this(), expect_adu, expect_adu.size());
    }
    virtual void write(const buffer_type& buf) override
    {}
    virtual void close() override
    {}
    virtual std::string remote_address() override
    {
      return "127.0.0.1";
    }
    virtual uint16_t remote_port() override
    {
      return 502;
    }

  private:
    read_handler rhandler_;
  };

  class read_test
  {
  public:
    void read(const buffer_type& pdu)
    {
      buffer_type expect_pdu{0x81, 0x02};
      EXPECT_EQ(pdu, expect_pdu);
    }
  };
  connection::pointer_type conn = std::make_shared<connection_test>();
  adu::adu_client c(conn);
  read_test t;
  c.set_handler(std::bind(&read_test::read, &t, std::placeholders::_1));
  conn->read();
}

} // modbus_test
} // asps_test

