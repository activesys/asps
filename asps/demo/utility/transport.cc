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

transport_service::pointer_type make_transport_service()
{
  return std::make_shared<transport>(*context);
}

void transport::connect(const connect_handler& handler)
{
  connect_handler_ = handler;
  ip::tcp::endpoint ep(ip::address::from_string(config::ip()), config::port());
  socket_.async_connect(ep, std::bind(&transport::on_connect, this, _1));
}

void transport::on_connect(const error_code& ec)
{
  connect_handler_(!ec);
}

void transport::write(const buffer_type& buf, const write_handler& handler)
{
  write_handler_ = handler;
  socket_.async_write_some(buffer(buf),
                           std::bind(&transport::on_write, this, _1, _2));
}

void transport::on_write(const error_code& ec, std::size_t bytes)
{
  write_handler_(!ec, bytes);
}

void transport::read(const read_handler& handler)
{
  read_handler_ = handler;
  socket_.async_read_some(buffer(read_buffer_),
                          std::bind(&transport::on_read, this, _1, _2));
}

void transport::on_read(const error_code& ec, std::size_t bytes)
{
  read_handler_(!ec, read_buffer_, bytes);
}

void transport::close()
{
  error_code ec;
  socket_.close(ec);
}

void transport::run()
{
  context_.run();
  context_.restart();
  context = std::make_shared<io_context>();
}

void transport::stop()
{
  context_.stop();
}

} // demo
} // asps
