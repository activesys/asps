// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo server.

#include <string>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <test/demo/utility/demo_test_client.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(demo_server_test, receive_datas)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept() override {}
    virtual void on_read(const data_group_type& datas) override
    {
      data_group_type expect_datas{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1001, 9001, 1648363193268),
        make_demo_data<int16_t>(1002, 9002, 1648363193268),
        make_demo_data<uint16_t>(1003, 9003, 1648363193268)
      };
      EXPECT_EQ(datas, expect_datas);
      stop();
      client_.stop();
    }

  private:
    demo_test_client& client_;
  };

  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x4d, // header length
    0x00, 0x04, // header count
    0x00, // header attr
    // mutable
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xe9, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xeb, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

} // demo_test
} // asps_test
