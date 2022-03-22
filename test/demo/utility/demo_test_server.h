// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo server for testing demo client.

#ifndef TEST_DEMO_UTILITY_DEMO_TEST_SERVER_H
#define TEST_DEMO_UTILITY_DEMO_TEST_SERVER_H

#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <boost/asio.hpp>

namespace asps_test {
namespace demo_test {

using namespace boost::asio;
using namespace std::placeholders;

class demo_test_server
{
public:
  demo_test_server(const std::string& host, uint16_t port)
    : acceptor_(context_, ip::tcp::endpoint(ip::address::from_string(host), port))
  {
    // A package must contain at least one piece of data.
    buffer_.resize(1024);
  }

public:
  void start(bool need_read_data = true)
  {
    need_read_data_ = need_read_data;
    server_thread_ = std::thread(std::bind(&demo_test_server::server_handler, this));
  }
  void wait()
  {
    //context_.stop();
    server_thread_.join();
  }
  const std::vector<uint8_t>& message()
  {
    return buffer_;
  }

private:
  void on_accept(const boost::system::error_code& ec)
  {
    if (!ec) {
      // Read Demo data
      if (need_read_data_) {
        async_read(*peer_,
                   buffer(buffer_),
                   std::bind(&demo_test_server::on_completion, this, _1, _2),
                   std::bind(&demo_test_server::on_read, this, _1, _2));
      }
    }
  }
  std::size_t on_completion(const boost::system::error_code& ec, std::size_t bytes)
  {
    // The header of demo protocol has a fixed length of 12 bytes
    if (bytes < 12) {
      return 12 - bytes;
    } else {
      // Parses the demo protocol package length,
      // expecting to return all demo package data.
      uint8_t* pos = buffer_.data();
      pos += 4;
      uint16_t length = ntohs(*reinterpret_cast<uint16_t*>(pos));
      
      if (bytes < length) {
        return length - bytes;
      } else {
        return 0;
      }
    }
  }
  void on_read(const boost::system::error_code& ec, std::size_t bytes)
  {
    buffer_.resize(bytes);
  }

  void server_handler()
  {
    peer_ = std::make_shared<ip::tcp::socket>(context_);
    acceptor_.async_accept(*peer_, std::bind(&demo_test_server::on_accept, this, _1));
    context_.run();
  }

private:
  std::thread server_thread_;
  io_context context_;
  ip::tcp::acceptor acceptor_;
  std::shared_ptr<ip::tcp::socket> peer_;
  std::vector<uint8_t> buffer_;
  bool need_read_data_;
};

} // demo_test
} // asps_test

#endif // TEST_DEMO_UTILITY_DEMO_SERVER_H
