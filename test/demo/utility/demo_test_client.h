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
#include <asps/demo/utility/utility.h>

namespace asps_test {
namespace demo_test {

using namespace boost::asio;
using namespace boost::system;
using namespace std::placeholders;
using namespace asps::demo;

class demo_test_client
{
public:
  demo_test_client(const std::string& ip, uint16_t port, const buffer_type& buf)
    : context_(),
      socket_(context_),
      ip_(ip),
      port_(port),
      buffer_(buf)
  {}

public:
  void start()
  {
    client_thread_ = std::thread(std::bind(&demo_test_client::client_handler, this));
  }
  void stop()
  {
    context_.stop();
    client_thread_.join();
  }

private:
  void do_connect()
  {
    ip::tcp::endpoint ep(ip::address::from_string(ip_), port_);
    socket_.async_connect(ep, std::bind(&demo_test_client::on_connect, this, _1));
  }
  void on_connect(const error_code& ec)
  {
    if (ec) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      do_connect();
    } else {
      do_write();
    }
  }
  void do_write()
  {
    socket_.async_write_some(buffer(buffer_), std::bind(&demo_test_client::on_write, this, _1, _2));
  }
  void on_write(const error_code& ec, std::size_t bytes)
  {}
  void client_handler()
  {
    do_connect();
    context_.run();
  }


private:
  std::thread client_thread_;
  io_context context_;
  ip::tcp::socket socket_;
  std::string ip_;
  uint16_t port_;
  buffer_type buffer_;
};

} // demo_test
} // asps_test

#endif // TEST_DEMO_UTILITY_DEMO_TEST_CLIENT_H
