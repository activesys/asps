// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for sequence.

#include <cstdint>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <asps/demo/sequence/sequence.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(data_sequence_test, request_and_response)
{
    data_group_type p{
    make_demo_data<bool>(1234, true, 1648001566463),
    make_demo_data<bool>(1235, true, 1648001566463),
    make_demo_data<bool>(1236, true, 1648001566463),
    make_demo_data<bool>(1237, true, 1648001566463),
    make_demo_data<bool>(1238, true, 1648001566463),
    make_demo_data<bool>(1239, true, 1648001566463),
    make_demo_data<bool>(1240, true, 1648001566463),
    make_demo_data<bool>(1241, true, 1648001566463),
    make_demo_data<bool>(1242, true, 1648001566463),
    make_demo_data<bool>(1243, true, 1648001566463),
    make_demo_data<bool>(1244, true, 1648001566463),
    make_demo_data<bool>(1245, true, 1648001566463),
    make_demo_data<bool>(1246, true, 1648001566463)
  };
  sequence_service::pointer_type ds = make_sequence_service(p);
  config::same_type(true);
  config::key_sequence(true);
  config::same_timestamp(true);

  const buffer_type& buffer = ds->request();
  const uint8_t expect_buffer[] = {
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x0d, // header count
    0x07, // header attr
    // mutable
    0x00, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    // datas
    0xff,
    0x1f
  };
  for (std::size_t i = 0; i < sizeof(expect_buffer); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }

  const uint8_t receive_buffer[] = {0x00};
  EXPECT_FALSE(ds->response(receive_buffer));
}

TEST(positive_keepalive_sequence_test, request_and_response)
{
  config::pack_nkeep(0xff, 0x00);
  sequence_service::pointer_type pks = make_sequence_service(true);

  const buffer_type& before_buffer = pks->request();
  const uint8_t expect_buffer[] = {
    0x4b, 0x45, 0x45, 0x50 // keep flag
  };
  for (std::size_t i = 0; i < sizeof(expect_buffer); ++i) {
    EXPECT_EQ(before_buffer[i], expect_buffer[i]);
  }

  const buffer_type& after_buffer = pks->request();
  EXPECT_TRUE(after_buffer.empty());

  uint8_t receive_buffer[] = {0xff};
  EXPECT_TRUE(pks->response(receive_buffer));
  EXPECT_FALSE(pks->response(receive_buffer));
}

TEST(negative_keepalive_sequence_test, request_and_response)
{
  config::pack_nkeep(0xff, 0x00);
  sequence_service::pointer_type nks = make_sequence_service(false);

  uint8_t receive_buffer[] = {0x00};
  EXPECT_TRUE(nks->response(receive_buffer));
  receive_buffer[0] = 0x88;
  EXPECT_FALSE(nks->response(receive_buffer));

  const buffer_type& buffer = nks->request();
  const uint8_t expect_buffer[] = {
    0x4b, 0x41, 0x43, 0x4b // kack flag
  };
  for (std::size_t i = 0; i < sizeof(expect_buffer); ++i) {
    EXPECT_EQ(buffer[i], expect_buffer[i]);
  }
}

TEST(get_sequence_type_test, get_sequence_type)
{
  config::pack_nkeep(0xff, 0x00);
  uint8_t buffer[] = {
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30
  };
  EXPECT_EQ(get_sequence_type(buffer), belong_to_data_sequence);

  buffer[0] = 0x4b;
  buffer[1] = 0x45;
  buffer[2] = 0x45;
  buffer[3] = 0x50;
  EXPECT_EQ(get_sequence_type(buffer), belong_to_positive_keepalive_sequence);

  buffer[0] = 0x4b;
  buffer[1] = 0x41;
  buffer[2] = 0x43;
  buffer[3] = 0x4b;
  EXPECT_EQ(get_sequence_type(buffer), belong_to_negative_keepalive_sequence);

  buffer[0] = 0xff;
  EXPECT_EQ(get_sequence_type(buffer), belong_to_positive_keepalive_sequence);

  buffer[0] = 0x00;
  EXPECT_EQ(get_sequence_type(buffer), belong_to_negative_keepalive_sequence);

  buffer[0] = 0xaa;
  EXPECT_EQ(get_sequence_type(buffer), belong_to_invalid_sequence);
}

} // demo_test
} // asps_test
