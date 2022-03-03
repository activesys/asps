// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Server.

#include <functional>
#include <asps/modbus/common/global_event.h>
#include <asps/modbus/api/server.h>
#include <asps/modbus/session/session.h>

using namespace asps::modbus;
using namespace std::placeholders;

// Modbus Server
void server::on_accept(const std::string& host, uint16_t port)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    event->on_accept(host, port);
  }
  // create session
  /*
  sessions_.emplace(std::make_pair(host, port),
                    std::make_shared<server_session>(transport_layer_));
                    */
}

void server::on_error(const std::string& error_message)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    event->on_error(error_message);
  }
}

void server::listen()
{
  transport_layer_.listen(std::bind(&server::on_accept, this, _1, _2),
                          std::bind(&server::on_error, this, _1));
}
  /*
  acceptor_.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket)
    {
      if (ec) {
        if (event_) {
          event_->on_accept(ec.message());
        }
      } else {
        tcp::endpoint endpoint = socket.remote_endpoint();
        if (event_) {
          event_->on_accept(endpoint.address().to_string(), endpoint.port());
        }
        std::make_shared<server_session>(
          std::move(socket))->start();
      }

      async_listen();
    });
    */

void server::event(server_event* e)
{
  global_server_event::instance()->event(e);
}

void server::run()
{
  transport_layer_.run();
}
