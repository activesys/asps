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

std::shared_ptr<io_context> g_context = std::make_shared<io_context>();

// Connection
connection::pointer_type
make_connection()
{
  return std::make_shared<boost_connection>(g_context);
}

void boost_connection::read(const read_handler& handler)
{
  read_handler_ = handler;
  socket_.async_read_some(buffer(buffer_),
                          std::bind(&boost_connection::on_read, this, _1, _2));
}

void boost_connection::write(const buffer_type& buf,
                             const write_handler& handler)
{
  write_handler_ = handler;
  socket_.async_write_some(buffer(buf),
                           std::bind(&boost_connection::on_write, this, _1, _2));
}

void boost_connection::close()
{
  socket_.close();
}

void boost_connection::on_read(const error_code& ec, std::size_t bytes)
{
  if (!ec) {
    read_handler_(shared_from_this(), buffer_, bytes);
  }

  socket_.async_read_some(buffer(buffer_),
                          std::bind(&boost_connection::on_read, this, _1, _2));
}

void boost_connection::on_write(const error_code& ec, std::size_t bytes)
{
  if (!ec) {
    write_handler_(shared_from_this(), bytes);
  }
}

// Connector
connector::pointer_type
make_connector(const std::string& ip, uint16_t port)
{
  return std::make_shared<boost_connector>(g_context, ip, port);
}

void boost_connector::connect(const connect_handler& handler)
{
  connect_handler_ = handler;
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
  connect_handler_(!ec, connection_);
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

void boost_acceptor::accept(const accept_handler& handler)
{
  accept_handler_ = handler;
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
  if (!ec) {
    accept_handler_(connection_);
  }

  do_accept();
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
