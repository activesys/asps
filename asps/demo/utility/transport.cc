// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Default Transport.

#include <asps/demo/config/config.h>
#include <asps/demo/utility/transport.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

std::shared_ptr<io_context> context = std::make_shared<io_context>();

// Client Transport
client_transport_service::pointer_type
make_client_transport_service()
{
  return std::make_shared<client_transport>(context);
}

void client_transport::connect(const connect_handler& handler)
{
  connect_handler_ = handler;
  ip::tcp::endpoint ep(ip::address::from_string(config::ip()), config::port());
  socket_.async_connect(ep, std::bind(&client_transport::on_connect, this, _1));
}

void client_transport::on_connect(const error_code& ec)
{
  connect_handler_(!ec);
}

void client_transport::write(const buffer_type& buf, const write_handler& handler)
{
  write_handler_ = handler;
  socket_.async_write_some(buffer(buf),
                           std::bind(&client_transport::on_write, this, _1, _2));
}

void client_transport::on_write(const error_code& ec, std::size_t bytes)
{
  write_handler_(!ec, bytes);
}

void client_transport::read(const read_handler& handler)
{
  read_handler_ = handler;
  socket_.async_read_some(buffer(read_buffer_),
                          std::bind(&client_transport::on_read, this, _1, _2));
}

void client_transport::on_read(const error_code& ec, std::size_t bytes)
{
  read_handler_(!ec, read_buffer_, bytes);
}

void client_transport::close()
{
  error_code ec;
  socket_.close(ec);
}

void client_transport::run()
{
  context_->run();
}

void client_transport::stop()
{
  context_->stop();
}

// Server Transport
server_transport_service::pointer_type
make_server_transport_service(const std::string& ip, uint16_t port)
{
  return std::make_shared<server_transport>(context, ip, port);
}

server_transport_service::pointer_type
make_server_transport_service(uint16_t port)
{
  return std::make_shared<server_transport>(context, port);
}

void server_transport::accept(const accept_handler& handler)
{
  accept_handler_ = handler;
  acceptor_.async_accept(peer_, std::bind(&server_transport::on_accept, this, _1));
}

void server_transport::read(const read_handler& handler)
{
  read_handler_ = handler;
  peer_.async_read_some(buffer(read_buffer_),
                        std::bind(&server_transport::on_read, this, _1, _2));
}

void server_transport::write(const buffer_type& buf, const write_handler& handler)
{
  write_handler_ = handler;
  peer_.async_write_some(buffer(buf),
                         std::bind(&server_transport::on_write, this, _1, _2));
}

void server_transport::run()
{
  context_->run();
}

void server_transport::stop()
{
  context_->stop();
}

void server_transport::close()
{
  peer_.close();
}

void server_transport::on_accept(const error_code& ec)
{
  accept_handler_(!ec);
}

void server_transport::on_read(const error_code& ec, std::size_t bytes)
{
  read_handler_(!ec, read_buffer_, bytes);
}

void server_transport::on_write(const error_code& ec, std::size_t bytes)
{
  write_handler_(!ec, bytes);
}

} // demo
} // asps
