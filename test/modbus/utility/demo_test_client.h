// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo client for testing demo server.

#ifndef TEST_DEMO_UTILITY_DEMO_TEST_CLIENT_H
#define TEST_DEMO_UTILITY_DEMO_TEST_CLIENT_H

#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <boost/asio.hpp>
#include <asps/utility/utility.hpp>

namespace asps_test {
namespace demo_test {

using namespace boost::asio;
using namespace boost::system;
using namespace asps::utility;

class demo_test_client
{
public:
  demo_test_client(const std::string& ip,
                   uint16_t port,
                   const buffer_type& buf,
                   std::size_t expect_length = 0)
    : context_(),
      socket_(context_),
      ip_(ip),
      port_(port),
      write_buffer_(buf),
      writed_bytes_(0),
      expect_length_(expect_length)
  {}

public:
  void start()
  {
    client_thread_ = std::thread(std::bind(&demo_test_client::client_handler, this));
  }
  void stop()
  {
    client_thread_.join();
  }

private:
  void do_connect()
  {
    ip::tcp::endpoint ep(ip::address::from_string(ip_), port_);
    socket_.async_connect(ep, std::bind(&demo_test_client::on_connect, this, std::placeholders::_1));
  }
  void on_connect(const error_code& ec)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (ec) {
      do_connect();
    } else if (expect_length_ > 0) {
      do_read();
    } else {
      do_write();
    }
  }
  void do_read()
  {
    read_buffer_.resize(expect_length_);
    socket_.async_read_some(buffer(read_buffer_), std::bind(&demo_test_client::on_read, this, std::placeholders::_1, std::placeholders::_2));
  }
  void on_read(const error_code& ec, std::size_t bytes)
  {
    do_write();
  }
  void do_write()
  {
    socket_.async_write_some(buffer(write_buffer_), std::bind(&demo_test_client::on_write, this, std::placeholders::_1, std::placeholders::_2));
  }
  void on_write(const error_code& ec, std::size_t bytes)
  {
    writed_bytes_ += bytes;
    /*
    if (writed_bytes_ >= write_buffer_.size()) {
      context_.stop();
    }
    */
  }
  void client_handler()
  {
    do_connect();
    context_.run();
  }

private:
  std::size_t expect_length_;
  std::thread client_thread_;
  io_context context_;
  ip::tcp::socket socket_;
  std::string ip_;
  uint16_t port_;
  buffer_type write_buffer_;
  buffer_type read_buffer_;
  std::size_t writed_bytes_;
};

} // demo_test
} // asps_test

#endif // TEST_DEMO_UTILITY_DEMO_TEST_CLIENT_H
