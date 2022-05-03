// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for server data.

#include <cstdint>
#include <vector>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

TEST(server_data_test, unserialize_float64_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x22, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x0a, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0xf1, 0xd6, 0x36, 0x76, 0x60, 0xe0, 0x2b, 0x41 // data value
  };
  data_group_type expect_datas{
    make_demo_data<double>(1234, 913456.230887143, 1647761782000)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_float32_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1e, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x09, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x71, 0x3d, 0xc4, 0xc2 // data value
  };
  data_group_type expect_datas{
    make_demo_data<float>(1234, -98.12, 1647761782000)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_int64_type)
{
  server_data d;
  buffer_type buffer{
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
  data_group_type expect_datas{
    make_demo_data<int64_t>(1234, -1154789657886957455, 1647761782000)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_uint32_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1e, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x06, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x00, 0x00, 0x26, 0x94 // data value
  };
  data_group_type expect_datas{
    make_demo_data<uint32_t>(1234, 9876, 1647761782000)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_int16_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1c, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x03, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xa6, 0x41, 0x74, 0xf0, // data timestamp
    0x26, 0x94 // data value
  };
  data_group_type expect_datas{
    make_demo_data<int16_t>(1234, 9876, 1647761782000)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_int8_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x01, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0xe0 // data value
  };
  data_group_type expect_datas{
    make_demo_data<int8_t>(1234, -32, 1648001566463)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_boolean_type)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x00, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x01 // data value
  };
  data_group_type expect_datas{
    make_demo_data<bool>(1234, true, 1648001566463)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_TRUE(buffer.empty());
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_multiple_types)
{
  server_data d;
  buffer_type buffer{
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
    // Redundant data
    0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
  };
  data_group_type expect_datas{
    make_demo_data<uint8_t>(1111, 11, 1648001566463),
    make_demo_data<int16_t>(1112, 12, 1648001566463),
    make_demo_data<uint32_t>(1113, 13, 1648001566463)
  };

  EXPECT_TRUE(d.unserialize(buffer));
  EXPECT_EQ(buffer.size(), 6);
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas, expect_datas);
}

TEST(server_data_test, unserialize_mutilated_header)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00
  };

  EXPECT_FALSE(d.unserialize(buffer));
  EXPECT_EQ(buffer.size(), 11);
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas.size(), 0);
}

TEST(server_data_test, unserialize_mutilated_package)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x00, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff // data timestamp
  };

  EXPECT_FALSE(d.unserialize(buffer));
  EXPECT_EQ(buffer.size(), 26);
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas.size(), 0);
}

TEST(server_data_test, unserialize_invalid_package)
{
  server_data d;
  buffer_type buffer{
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x31, 0x31, // invalid header flag
    0x00, 0x1b, // header length
    0x00, 0x01, // header count
    0x00, // header attr
    // mutable
    // data
    0x00, // data type
    0x00, 0x00, 0x04, 0xd2, // data key
    0x00, 0x00, 0x01, 0x7f, 0xb4, 0x8c, 0x46, 0xff, // data timestamp
    0x00
  };

  EXPECT_FALSE(d.unserialize(buffer));
  EXPECT_EQ(buffer.size(), 27);
  const data_group_type& datas = d.datas();
  EXPECT_EQ(datas.size(), 0);
}

} // demo_test
} // asps_test
