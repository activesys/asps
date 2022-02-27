// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Server.

#include <asps/modbus/api/server.h>
#include <asps/modbus/session/session.h>

using namespace asps::modbus;

// Modbus Server
void server::async_listen()
{
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
          std::move(socket), sessions_)->start();
      }

      async_listen();
    });
}

void server::register_event(server_event* event)
{
  event_ = event;
}

void server::run()
{
  context_.run();
}
