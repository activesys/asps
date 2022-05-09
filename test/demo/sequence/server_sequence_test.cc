// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for server sequence.

#include <cstdint>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <asps/demo/sequence/server_sequence.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(server_data_sequence_test, receive_and_notify)
{
  class data_observer_test : public data_observer
  {
  public:
    data_observer_test()
      : count(0)
    {}

  public:
    virtual void update_data(const data_group_type& datas) override
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
    0xef, 0xf9, 0x5c, 0xec, 0xab, 0x5a, 0x60, 0x71 // data value
  };

  server_data_sequence seq;
  data_observer_test dot;
  seq.register_data_observer(&dot);

  EXPECT_EQ(buffer.size(), 93);
  EXPECT_TRUE(seq.receive_data(buffer));
  EXPECT_EQ(buffer.size(), 34);
  EXPECT_TRUE(seq.receive_data(buffer));
  EXPECT_EQ(buffer.size(), 0);
}

TEST(server_positive_keepalive_sequence_test, request_and_response)
{
  config::pack_nkeep(0xff, 0x00);
  passive_sequence_service::pointer_type pks = make_passive_sequence(true);

  std::vector<uint8_t> receive_buffer{0x4c, 0x45, 0x45, 0x50};
  EXPECT_FALSE(pks->receive_request(receive_buffer));
  EXPECT_EQ(receive_buffer.size(), 4);
  receive_buffer[0] = 0x4b;
  EXPECT_TRUE(pks->receive_request(receive_buffer));
  EXPECT_EQ(receive_buffer.size(), 0);

  const buffer_type& buffer = pks->send_response();
  const uint8_t expect_buffer[] = {
    0xff // kack flag
  };
  for (std::size_t i = 0; i < sizeof(expect_buffer); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(server_negative_keepalive_sequence_test, request_and_response)
{
  config::pack_nkeep(0xff, 0x00);
  active_sequence_service::pointer_type pks = make_active_sequence(true);

  const buffer_type& before_buffer = pks->send_request();
  const uint8_t expect_buffer[] = {
    0x00
  };
  for (std::size_t i = 0; i < sizeof(expect_buffer); ++i) {
    EXPECT_EQ(before_buffer[i], expect_buffer[i]);
  }

  const buffer_type& after_buffer = pks->send_request();
  EXPECT_TRUE(after_buffer.empty());

  std::vector<uint8_t> receive_buffer{
    0x4b, 0x41, 0x43, 0x4b, 0x4b, 0x41, 0x43, 0x4b // kack flag // kack flag
  };
  EXPECT_TRUE(pks->receive_response(receive_buffer));
  EXPECT_TRUE(pks->receive_response(receive_buffer));
}

TEST(get_server_sequence_type_test, get_sequence_type)
{
  config::pack_nkeep(0xff, 0x00);
  std::vector<uint8_t> buffer{
    // header flag 'DEMOV100'
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30
  };
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_data_sequence);

  // positive keepalive flag 'KEEP'
  buffer[0] = 0x4b;
  buffer[1] = 0x45;
  buffer[2] = 0x45;
  buffer[3] = 0x50;
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_positive_keepalive_sequence);

  // negative keepalive ack flag 'KACK'
  buffer[0] = 0x4b;
  buffer[1] = 0x41;
  buffer[2] = 0x43;
  buffer[3] = 0x4b;
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_negative_keepalive_sequence);

  // positive keepalive ack
  buffer[0] = 0xff;
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_invalid_sequence);

  // negative keepalive
  buffer[0] = 0x00;
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_invalid_sequence);

  // invalid message
  buffer[0] = 0xaa;
  EXPECT_EQ(get_server_sequence_type(buffer), belong_to_invalid_sequence);
}

} // demo_test
} // asps_test
