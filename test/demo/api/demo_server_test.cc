// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for demo server.

#include <string>
#include <gtest/gtest.h>
#include <asps/demo/demo.h>
#include <asps/demo/utility/transport.h>
#include <test/demo/utility/demo_test_client.h>

namespace asps_test {
namespace demo_test {

using namespace asps::demo;

class demo_server_test : public ::testing::Test
{
protected:
  demo_server_test()
  {
    g_context = std::make_shared<io_context>();
  }
};

TEST_F(demo_server_test, receive_datas)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override {}
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
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

TEST_F(demo_server_test, receive_datas_with_compress_same_type)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
    {
      data_group_type expect_datas{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1002, 9001, 1648363193268),
        make_demo_data<int32_t>(1004, 9002, 1648363193268),
        make_demo_data<int32_t>(1006, 9003, 1648363193268)
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
    0x00, 0x4e, // header length
    0x00, 0x04, // header count
    0x01, // header attr
    // mutable
    0x05,
    // data 1
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x03, 0xee, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, receive_datas_with_compress_same_timestamp)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
    {
      data_group_type expect_datas{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1002, 9001, 1648363193268),
        make_demo_data<int16_t>(1004, 9002, 1648363193268),
        make_demo_data<uint16_t>(1006, 9003, 1648363193268)
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
    0x00, 0x35, // header length
    0x00, 0x04, // header count
    0x04, // header attr
    // mutable
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x03, 0xea, // data key
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x03, 0xee, // data key
    0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, receive_datas_with_compress_key_sequence)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read_raw(const connection::pointer_type conn,
                            const buffer_type& buffer,
                            std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
    {
      data_group_type expect_datas{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<uint32_t>(1001, 9001, 1648363193269),
        make_demo_data<int16_t>(1002, 9002, 1648363193270),
        make_demo_data<uint16_t>(1003, 9003, 1648363193271)
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
    0x00, 0x41, // header length
    0x00, 0x04, // header count
    0x02, // header attr
    // mutable
    0x00, 0x00, 0x03, 0xe8, // data key
    // data 1
    0x05, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x06, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb5, // data timestamp
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x03, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    0x23, 0x2a, // data value
    // data 4
    0x04, // data type
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb7, // data timestamp
    0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, receive_datas_with_compress_all_attribute)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                        const data_group_type& datas) override
    {
      data_group_type expect_datas{
        make_demo_data<int32_t>(1000, 9000, 1648363193268),
        make_demo_data<int32_t>(1001, 9001, 1648363193268),
        make_demo_data<int32_t>(1002, 9002, 1648363193268),
        make_demo_data<int32_t>(1003, 9003, 1648363193268)
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
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, receive_datas_with_compress_multiple_packages)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c),
        count_(0)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
    {
      if (count_ == 0) {
        data_group_type expect_datas{
          make_demo_data<int32_t>(1000, 9000, 1648363193268),
          make_demo_data<int32_t>(1001, 9001, 1648363193268),
          make_demo_data<int32_t>(1002, 9002, 1648363193268),
          make_demo_data<int32_t>(1003, 9003, 1648363193268)
        };
        EXPECT_EQ(datas, expect_datas);
        count_++;
      } else if (count_ == 1) {
        data_group_type expect_datas{
          make_demo_data<int32_t>(1004, 9000, 1648363193270),
          make_demo_data<int32_t>(1005, 9001, 1648363193270),
          make_demo_data<int32_t>(1006, 9002, 1648363193270),
          make_demo_data<int32_t>(1007, 9003, 1648363193270)
        };
        EXPECT_EQ(datas, expect_datas);
        count_++;
      } else {
        data_group_type expect_datas{
          make_demo_data<uint32_t>(1000, 9000, 1648363193270),
          make_demo_data<uint32_t>(1001, 9001, 1648363193270),
          make_demo_data<uint32_t>(1002, 9002, 1648363193270),
          make_demo_data<uint32_t>(1003, 9003, 1648363193270)
        };
        EXPECT_EQ(datas, expect_datas);
        count_++;
        stop();
        client_.stop();
      }
    }

  private:
    demo_test_client& client_;
    int count_;
  };

  buffer_type buffer{
    // package 1: t int32_t, k 1000, T 1648363193268
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb4, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b, // data value
    // package 2: t int32_t, k 1004, T 1648363193270
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x05, // data type
    0x00, 0x00, 0x03, 0xec, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b, // data value
    // package 3: t uint32_t, k 1000, T 1648363193270
    // header
    0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30, // header flag
    0x00, 0x2a, // header length
    0x00, 0x04, // header count
    0x07, // header attr
    // mutable
    0x06, // data type
    0x00, 0x00, 0x03, 0xe8, // data key
    0x00, 0x00, 0x01, 0x7f, 0xca, 0x1a, 0x43, 0xb6, // data timestamp
    // data 1
    0x00, 0x00, 0x23, 0x28, // data value
    // data 2
    0x00, 0x00, 0x23, 0x29, // data value
    // data 3
    0x00, 0x00, 0x23, 0x2a, // data value
    // data 4
    0x00, 0x00, 0x23, 0x2b // data value
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, server_positive_keepalive)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_read(const connection::pointer_type conn,
                        const data_group_type& datas) override
    {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override
    {}
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override
    {
      EXPECT_EQ(bytes, 1);
      stop();
      client_.stop();
    }

  private:
    demo_test_client& client_;
  };

  buffer_type buffer{
    // positive keepalive flag 'KEEP'
    0x4b, 0x45, 0x45, 0x50
  };

  demo_test_client c("127.0.0.1", 9900, buffer);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

TEST_F(demo_server_test, server_negative_keepalive)
{
  class my_demo_server : public demo_server
  {
  public:
    my_demo_server(const std::string& ip, uint16_t port, demo_test_client& c)
      : demo_server(ip, port),
        client_(c)
    {}

  public:
    virtual void on_accept(const connection::pointer_type conn) override {}
    virtual void on_read(const connection::pointer_type conn,
                         const data_group_type& datas) override
    {}
    virtual void on_read_raw(const connection::pointer_type conn,
                             const buffer_type& buffer,
                             std::size_t bytes) override
    {
      buffer_type expect_buffer{
        // positive keepalive flag 'KACK'
        0x4b, 0x41, 0x43, 0x4b
      };
      for (std::size_t i = 0; i < expect_buffer.size(); ++i) {
        EXPECT_EQ(buffer[i], expect_buffer[i]);
      }
      stop();
      client_.stop();
    }
    virtual void on_write_raw(const connection::pointer_type conn,
                              const buffer_type& buffer,
                              std::size_t bytes) override
    {
      EXPECT_EQ(bytes, 1);
    }

  private:
    demo_test_client& client_;
  };

  buffer_type buffer{
    // positive keepalive flag 'KACK'
    0x4b, 0x41, 0x43, 0x4b
  };

  config::t1_t2(3, 2);
  demo_test_client c("127.0.0.1", 9900, buffer, 1);
  c.start();

  my_demo_server mds("0.0.0.0", 9900, c);
  mds.run();
}

} // demo_test
} // asps_test
