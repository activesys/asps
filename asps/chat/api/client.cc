// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Client.

#include <memory>
#include <boost/bind.hpp>
#include <asps/chat/api/client.h>

using namespace asps::chat;

// Chat Client
void client::register_event(client_event* event)
{
  event_ = event;
}

void client::async_connect()
{
  boost::system::error_code ec;
  boost::asio::async_connect(
    socket_,
    endpoint_,
    [this](boost::system::error_code ec, tcp::resolver::endpoint_type endpoint)
    {
      if (ec) {
        if (event_) {
          event_->on_connect(ec.message());
        }
      } else {
        session_ = std::make_shared<client_session>(socket_, event_);
        session_->start();

        if (event_) {
          event_->on_connect(endpoint.address().to_string(), endpoint.port());
        }
      }
    });
}

void client::close()
{
  socket_.close();
}

uint16_t client::async_send(const std::string& msg)
{
  return session_->send(msg);
}

void client::run()
{
  context_.run();
}
