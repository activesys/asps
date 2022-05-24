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
#include <asps/utility/utility.h>

namespace asps_test {
namespace demo_test {

using namespace asps::utility;
using namespace boost::asio;

class demo_test_server
{
public:
  demo_test_server(const std::string& host, uint16_t port)
    : acceptor_(context_, ip::tcp::endpoint(ip::address::from_string(host), port)),
      expect_length_(0)
  {
    // A package must contain at least one piece of data.
    buffer_.resize(1024);
  }

public:
  void expect_length(std::size_t length) {expect_length_ = length;}
  void write_data(const std::vector<uint8_t>& buf) {write_buffer_ = buf;}
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
                   std::bind(&demo_test_server::on_completion, this, std::placeholders::_1, std::placeholders::_2),
                   std::bind(&demo_test_server::on_read, this, std::placeholders::_1, std::placeholders::_2));
      }
      // Write Data
      if (write_buffer_.size()) {
        async_write(*peer_,
                    buffer(write_buffer_),
                    std::bind(&demo_test_server::on_write, this, std::placeholders::_1, std::placeholders::_2));
      }
    }
  }
  void on_write(const boost::system::error_code& ec, std::size_t bytes)
  {
    write_buffer_.clear();
  }
  std::size_t on_completion(const boost::system::error_code& ec, std::size_t bytes)
  {
    if (expect_length_ == SIZE_MAX) {
      return 0;
    }

    if (expect_length_ > 0 && bytes < expect_length_) {
      return expect_length_ - bytes;
    }

    // for KEEP and KACK
    if (expect_length_ == 4 && bytes == 4) {
      return 0;
    }

    // The header of demo protocol has a fixed length of 16 bytes
    if (bytes < 13) {
      return 13 - bytes;
    } else {
      // Parses the demo protocol package length,
      // expecting to return all demo package data.
      uint8_t* pos = buffer_.data();
      pos += 8;
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
    acceptor_.async_accept(*peer_, std::bind(&demo_test_server::on_accept, this, std::placeholders::_1));
    context_.run();
  }

private:
  std::size_t expect_length_;
  std::thread server_thread_;
  io_context context_;
  ip::tcp::acceptor acceptor_;
  std::shared_ptr<ip::tcp::socket> peer_;
  std::vector<uint8_t> buffer_;
  std::vector<uint8_t> write_buffer_;
  bool need_read_data_;
};

} // demo_test
} // asps_test

#endif // TEST_DEMO_UTILITY_DEMO_SERVER_H
