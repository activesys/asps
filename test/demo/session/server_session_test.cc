// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for server session.

#include <cstdint>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <asps/demo/session/server_session.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(server_session_test, receive_data_and_notify)
{
  class server_observer_test : public server_observer
  {
  public:
    server_observer_test()
      : count(0)
    {}

  public:
    virtual void update_event(server_session_service::pointer_type session) override
    {}
    virtual void update_send(server_session_service::pointer_type session,
                             const buffer_type& buffer) override
    {}
    virtual void update_data(server_session_service::pointer_type session,
                             const data_group_type& datas) override
    {
      if (count == 0) {
        data_group_type expect_datas{
          make_demo_data<uint8_t>(1111, 11, 1648001566463),
          make_demo_data<int16_t>(1112, 12, 1648001566463),
          make_demo_data<uint32_t>(1113, 13, 1648001566463)
        };
        EXPECT_EQ(datas, expect_datas);

        count++;
      } else {
        data_group_type expect_datas{
          make_demo_data<int64_t>(1234, -1154789657886957455, 1647761782000)
        };
        EXPECT_EQ(datas, expect_datas);
      }
    }

  private:
    int count;
  };

  buffer_type buffer{
    // package 1
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x3b, // header length
    0x00, 0x03, // header count
    0x00, // header attr
    // mutable
    // data 1
    0x02, // data type
    0x00, 0x00, 0x04, 0x57, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x0b,  // data value
    // data 2
    0x03, // data type
    0x00, 0x00, 0x04, 0x58, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x00, 0x0c,  // data value
    // data 3
    0x06, // data type
    0x00, 0x00, 0x04, 0x59, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x00, 0x00, 0x00, 0x0d,  // data value
    
    // package 2
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x22, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x07, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0xef, 0xf9, 0x5c, 0xec, 0xab, 0x5a, 0x60, 0x71, // data value
    // invalid data
    0xaa, 0xff
  };

  server_session_service::pointer_type session = make_server_session();
  server_observer_test sot;
  session->register_observer(&sot);

  EXPECT_EQ(buffer.size(), 95);
  session->receive(buffer);
  EXPECT_EQ(buffer.size(),0);
}

TEST(server_session_test, receive_positive_keepalive_and_notify)
{
  class server_observer_test : public server_observer
  {
  public:
    server_observer_test()
      : count(0)
    {}

  public:
    virtual void update_send(server_session_service::pointer_type session,
                             const buffer_type& buffer) override
    {
      EXPECT_EQ(buffer.size(), 1);
      EXPECT_EQ(buffer[0], 0xff);
    }
    virtual void update_data(server_session_service::pointer_type session,
                             const data_group_type& datas) override
    {}
    virtual void update_event(server_session_service::pointer_type session)
    {}

  private:
    int count;
  };

  buffer_type buffer{
    // positive keepalive flag 'KEEP'
    0x4b, 0x45, 0x45, 0x50
  };

  config::pack_nkeep(0xff, 0x00);
  server_session_service::pointer_type session = make_server_session();
  server_observer_test sot;
  session->register_observer(&sot);

  EXPECT_EQ(buffer.size(), 4);
  session->receive(buffer);
  EXPECT_EQ(buffer.size(), 0);
}

} // demo_test
} // asps_test
