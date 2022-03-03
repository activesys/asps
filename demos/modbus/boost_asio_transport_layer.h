// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Implement transport Layer with Boost ASio.

#ifndef DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
#define DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H

#include <vector>
#include <boost/asio.hpp>
#include <asps/modbus/api/transport_layer.h>

namespace asps_demos {
namespace modbus_demos {

using boost::asio::ip::tcp;
using namespace asps::modbus;

class boost_asio_transport_layer : public transport_layer
{
public:
  boost_asio_transport_layer(const std::string& host, uint16_t port = 502)
    : context_(),
      socket_(context_),
      acceptor_(context_, tcp::endpoint(tcp::v4(), port))
  {
    tcp::resolver resolver(context_);
    endpoint_ = resolver.resolve(host, std::to_string(port));
  }

public:
  void listen(accept_handler on_accept, error_handler on_error) override
  {
    acceptor_.async_accept(
      [=](boost::system::error_code ec, tcp::socket socket)
      {
        if (ec) {
          on_error(ec.message());
        } else {
          tcp::endpoint endpoint = socket.remote_endpoint();
          on_accept(endpoint.address().to_string(), endpoint.port());
        }

        listen(on_accept, on_error);
      });
  }

  void connect(connect_handler on_connect, error_handler on_error) override
  {
    boost::system::error_code ec;
    boost::asio::async_connect(
      socket_,
      endpoint_,
      [=](boost::system::error_code ec, tcp::resolver::endpoint_type endpoint)
      {
        if (ec) {
          on_error(ec.message());
        } else {
          on_connect(endpoint.address().to_string(), endpoint.port());
        }
      });
  }

  void write(const uint8_t* buffer,
             std::size_t length,
             eof_handler on_eof,
             error_handler on_error) override
  {
    boost::asio::async_write(
      socket_,
      boost::asio::buffer(buffer, length),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (ec == boost::asio::error::eof) {
          on_eof();
        } else if (ec) {
          on_error(ec.message());
        }
      });
  }

  void read(std::size_t length,
            read_handler on_read,
            eof_handler on_eof,
            error_handler on_error) override
  {
    std::size_t remain_length = buffer_.size();
    buffer_.resize(length);
    boost::asio::async_read(
      socket_,
      boost::asio::buffer(buffer_.data() + remain_length,
                          buffer_.size() - remain_length),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          on_read(buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          on_eof();
        } else {
          on_error(ec.message());
        }
      });
  }

  void glance(std::size_t length,
              glance_handler on_glance,
              eof_handler on_eof,
              error_handler on_error) override
  {
    buffer_.resize(length);
    boost::asio::async_read(
      socket_,
      boost::asio::buffer(buffer_.data(), buffer_.size()),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          on_glance(buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          on_eof();
        } else {
          on_error(ec.message());
        }
      });
  }

  void run() override
  {
    context_.run();
  }

  void close() override
  {
    socket_.close();
  }

private:
  boost::asio::io_context context_;
  tcp::socket socket_;
  tcp::acceptor acceptor_;
  tcp::resolver::results_type endpoint_;
  std::vector<uint8_t> buffer_;
};

} // modbus_demos
} // asps_demos

#endif // DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
