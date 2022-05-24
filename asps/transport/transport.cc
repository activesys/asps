// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Transport layer services implemented using Boost.

#include <asps/transport/transport.h>
#include <asps/utility/boost_env.h>

namespace asps {
namespace transport {

using namespace std::placeholders;
using namespace asps::utility;

// Connection
connection::pointer_type
make_connection()
{
  return std::make_shared<boost_connection>(g_context);
}

void boost_connection::set_handler(const read_handler rhandler,
                                   const write_handler whandler,
                                   const close_handler chandler)
{
  read_handler_ = rhandler;
  write_handler_ = whandler;
  close_handler_ = chandler;
}

void boost_connection::read()
{
  socket_.async_read_some(buffer(buffer_),
                          std::bind(&boost_connection::on_read, this, _1, _2));
}

void boost_connection::write(const buffer_type& buf)
{
  async_write(socket_,
              buffer(buf),
              std::bind(&boost_connection::on_write, this, _1, _2));
}

void boost_connection::close()
{
  error_code ec;
  socket_.close(ec);
}

void boost_connection::on_read(const error_code& ec, std::size_t bytes)
{
  if (ec == error::eof && close_handler_) {
    close_handler_(shared_from_this());
  } else if (!ec && read_handler_) {
    read_handler_(shared_from_this(), buffer_, bytes);
  } else if (ec != error::bad_descriptor) {
    socket_.async_read_some(buffer(buffer_),
                            std::bind(&boost_connection::on_read, this, _1, _2));
  }
}

void boost_connection::on_write(const error_code& ec, std::size_t bytes)
{
  if (ec == error::eof && close_handler_) {
    close_handler_(shared_from_this());
  } else if (!ec && write_handler_) {
    write_handler_(shared_from_this(), bytes);
  }
}

std::string boost_connection::remote_address()
{
  if (remote_ip_.empty()) {
    error_code ec;
    ip::tcp::endpoint ep = socket_.remote_endpoint(ec);
    if (!ec) {
      remote_ip_ = ep.address().to_string();
    }
  }
  return remote_ip_;
}

uint16_t boost_connection::remote_port()
{
  if (remote_port_ == 0) {
    error_code ec;
    ip::tcp::endpoint ep = socket_.remote_endpoint(ec);
    if (!ec) {
      remote_port_ = ep.port();
    }
  }
  return remote_port_;
}

// Connector
connector::pointer_type
make_connector(const std::string& ip, uint16_t port)
{
  return std::make_shared<boost_connector>(g_context, ip, port);
}

void boost_connector::set_handler(const connect_handler handler)
{
  connect_handler_ = handler;
}

void boost_connector::connect()
{
  do_connect();
}

void boost_connector::do_connect()
{
  boost_connection* conn = dynamic_cast<boost_connection*>(connection_.get());
  if (conn != nullptr) {
    ip::tcp::endpoint ep(ip::address::from_string(ip_), port_);
    conn->socket().async_connect(ep,
                                 std::bind(&boost_connector::on_connect,
                                           this,
                                           _1));
  }
}

void boost_connector::on_connect(const error_code& ec)
{
  if (!ec && connect_handler_) {
    connect_handler_(connection_);
  }
}

void boost_connector::run()
{
  context_->run();
}

void boost_connector::stop()
{
  context_->stop();
}

// Acceptor
acceptor::pointer_type
make_acceptor(const std::string& ip, uint16_t port)
{
  return std::make_shared<boost_acceptor>(g_context, ip, port);
}

acceptor::pointer_type
make_acceptor(uint16_t port)
{
  return std::make_shared<boost_acceptor>(g_context, port);
}

void boost_acceptor::set_handler(const accept_handler ahandler,
                                 const release_handler rhandler)
{
  accept_handler_ = ahandler;
  release_handler_ = rhandler;
}

void boost_acceptor::accept()
{
  do_accept();
}

void boost_acceptor::do_accept()
{
  connection_ = make_connection();
  boost_connection* conn = dynamic_cast<boost_connection*>(connection_.get());
  if (conn != nullptr) {
    acceptor_.async_accept(conn->socket(),
                           std::bind(&boost_acceptor::on_accept, this, _1));
  }
}

void boost_acceptor::on_accept(const error_code& ec)
{
  if (!ec && accept_handler_) {
    accept_handler_(connection_);
  }

  do_accept();
}

void boost_acceptor::on_release(connection::pointer_type conn)
{
  if (conn && release_handler_) {
    release_handler_(conn);
  }
}

void boost_acceptor::release(connection::pointer_type conn)
{
  context_->post(std::bind(&boost_acceptor::on_release, this, conn));
}

void boost_acceptor::run()
{
  context_->run();
}

void boost_acceptor::stop()
{
  context_->stop();
}

} // demo
} // asps
