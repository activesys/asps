// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Implement transport Layer with Boost ASio.

#ifndef DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
#define DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H

#include <vector>
#include <map>
#include <boost/asio.hpp>
#include <asps/modbus/api/transport_layer.h>

namespace asps_demos {
namespace modbus_demos {

using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace asps::modbus;

class boost_asio_client_transport_layer : public client_transport_layer
{
public:
  boost_asio_client_transport_layer(client_transport_event& event,
                                    const std::string& host,
                                    uint16_t port = 502)
    : client_transport_layer(event),
      context_(),
      socket_(context_)
  {
    tcp::resolver resolver(context_);
    endpoint_ = resolver.resolve(host, std::to_string(port));
  }

public:
  virtual void connect() override
  {
    boost::system::error_code ec;
    boost::asio::async_connect(
      socket_,
      endpoint_,
      [=](boost::system::error_code ec, tcp::resolver::endpoint_type endpoint)
      {
        if (ec) {
          event_.on_error(ec.message());
        } else {
          event_.on_connect(endpoint.address().to_string(), endpoint.port());
        }
      });
  }

  virtual void write(const uint8_t* buffer, std::size_t length) override
  {
    boost::asio::async_write(
      socket_,
      boost::asio::buffer(buffer, length),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (ec == boost::asio::error::eof) {
          event_.on_eof();
        } else if (ec) {
          event_.on_error(ec.message());
        }
      });
  }

  virtual void read(std::size_t length) override
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
          event_.on_read(buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          event_.on_eof();
        } else {
          event_.on_error(ec.message());
        }
      });
  }

  virtual void glance(std::size_t length) override
  {
    buffer_.resize(length);
    boost::asio::async_read(
      socket_,
      boost::asio::buffer(buffer_.data(), buffer_.size()),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          event_.on_glance(buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          event_.on_eof();
        } else {
          event_.on_error(ec.message());
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
  tcp::resolver::results_type endpoint_;
  std::vector<uint8_t> buffer_;
};

class boost_asio_server_transport_layer : public server_transport_layer
{
public:
  boost_asio_server_transport_layer(server_transport_event& event,
                                    const std::string& host,
                                    uint16_t port = 502)
    : server_transport_layer(event),
      context_(),
      socket_(context_),
      acceptor_(context_, tcp::endpoint(tcp::v4(), port))
  {
    tcp::resolver resolver(context_);
    endpoint_ = resolver.resolve(host, std::to_string(port));
  }

public:
  void listen() override
  {
    acceptor_.async_accept(
      [=](boost::system::error_code ec, tcp::socket socket)
      {
        if (ec) {
          event_.on_error("", 0, ec.message());
        } else {
          tcp::endpoint endpoint = socket.remote_endpoint();
          peers_[endpoint] = std::make_shared<tcp::socket>(std::move(socket));
          event_.on_accept(endpoint.address().to_string(), endpoint.port());
        }

        listen();
      });
  }

  void write(const std::string& host, uint16_t port, const uint8_t* buffer, std::size_t length) override
  {
    tcp::endpoint endpoint;
    endpoint.address(ip::make_address(host));
    endpoint.port(port);
    std::shared_ptr<tcp::socket> sock = peers_[endpoint];

    boost::asio::async_write(
      *sock,
      boost::asio::buffer(buffer, length),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (ec == boost::asio::error::eof) {
          event_.on_eof(host, port);
          close(host, port);
        } else if (ec) {
          event_.on_error(host, port, ec.message());
          close(host, port);
        }
      });
  }

  void read(const std::string& host, uint16_t port, std::size_t length) override
  {
    tcp::endpoint endpoint;
    endpoint.address(ip::make_address(host));
    endpoint.port(port);
    std::shared_ptr<tcp::socket> sock = peers_[endpoint];
    std::size_t remain_length = buffer_.size();

    buffer_.resize(length);
    boost::asio::async_read(
      *sock,
      boost::asio::buffer(buffer_.data() + remain_length,
                          buffer_.size() - remain_length),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          event_.on_read(host, port, buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          event_.on_eof(host, port);
          close(host, port);
        } else {
          event_.on_error(host, port, ec.message());
          close(host, port);
        }
      });
  }

  void glance(const std::string& host, uint16_t port, std::size_t length) override
  {
    tcp::endpoint endpoint;
    endpoint.address(ip::make_address(host));
    endpoint.port(port);
    std::shared_ptr<tcp::socket> sock = peers_[endpoint];

    buffer_.resize(length);
    boost::asio::async_read(
      *sock,
      boost::asio::buffer(buffer_.data(), buffer_.size()),
      [=](boost::system::error_code ec, std::size_t length)
      {
        if (!ec) {
          event_.on_glance(host, port, buffer_.data());
        } else if (ec == boost::asio::error::eof) {
          event_.on_eof(host, port);
          close(host, port);
        } else {
          event_.on_error(host, port, ec.message());
          close(host, port);
        }
      });
  }

  void run() override
  {
    context_.run();
  }

  void close(const std::string& host, uint16_t port) override
  {
    tcp::endpoint endpoint;
    endpoint.address(ip::make_address(host));
    endpoint.port(port);
    peers_.erase(endpoint);
  }

private:
  boost::asio::io_context context_;
  tcp::socket socket_;
  tcp::acceptor acceptor_;
  tcp::resolver::results_type endpoint_;
  std::vector<uint8_t> buffer_;
  std::map<tcp::endpoint, std::shared_ptr<tcp::socket>> peers_;
};

} // modbus_demos
} // asps_demos

#endif // DEMOS_MODBUS_BOOST_ASIO_TRANSPORT_LAYER_H
