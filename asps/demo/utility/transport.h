// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Default Transport.

#ifndef ASPS_DEMO_UTILITY_TRANSPORT_H
#define ASPS_DEMO_UTILITY_TRANSPORT_H

#include <memory>
#include <boost/asio.hpp>
#include <asps/demo/api/transport_service.h>

namespace asps {
namespace demo {

using namespace boost::asio;
using namespace boost::system;

extern std::shared_ptr<io_context> context;

// Client Transport
class client_transport : public client_transport_service
{
public:
  client_transport(std::shared_ptr<io_context> context)
    : client_transport_service(),
      context_(context),
      socket_(*context_)
  {
    read_buffer_.resize(config::read_buffer_size());
  }

public:
  void connect(const connect_handler& handler) override;
  void close() override;
  void read(const read_handler& handler) override;
  void write(const buffer_type& buf, const write_handler& handler) override;
  void run() override;
  void stop() override;

private:
  void on_connect(const error_code& ec);
  void on_write(const error_code& ec, std::size_t bytes);
  void on_read(const error_code& ec, std::size_t bytes);

private:
  std::shared_ptr<io_context> context_;
  ip::tcp::socket socket_;
  connect_handler connect_handler_;
  write_handler write_handler_;
  read_handler read_handler_;
  buffer_type read_buffer_;
};

// Server Transport
class server_transport : public server_transport_service
{
public:
  server_transport(std::shared_ptr<io_context> context, const std::string& ip, uint16_t port)
    : context_(context),
      acceptor_(*context_, ip::tcp::endpoint(ip::address::from_string(ip), port)),
      peer_(*context_)
  {
    read_buffer_.resize(config::read_buffer_size());
  }
  server_transport(std::shared_ptr<io_context> context, uint16_t port)
    : context_(context),
      acceptor_(*context_, ip::tcp::endpoint(ip::tcp::v4(), port)),
      peer_(*context_)
  {
    read_buffer_.resize(config::read_buffer_size());
  }

public:
  virtual void accept(const accept_handler& handler) override;
  virtual void read(const read_handler& handler) override;
  virtual void write(const buffer_type& buf, const write_handler& handler) override;
  virtual void stop() override;
  virtual void run() override;
  virtual void close() override;

private:
  void on_accept(const error_code& ec);
  void on_read(const error_code& ec, std::size_t bytes);
  void on_write(const error_code& ec, std::size_t bytes);

private:
  std::shared_ptr<io_context> context_;
  ip::tcp::acceptor acceptor_;
  ip::tcp::socket peer_;
  buffer_type read_buffer_;
  read_handler read_handler_;
  accept_handler accept_handler_;
  write_handler write_handler_;
};

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_TRANSPORT_H
