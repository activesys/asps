// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Client.

#include <memory>
#include <boost/bind.hpp>
#include <asps/chat/client.h>

using namespace asps::chat;

// Chat Client
bool client::connect()
{
  boost::system::error_code ec;
  boost::asio::connect(socket_, endpoints_, ec);
  if (!ec) {
    session_ = std::make_shared<client_session>(socket_, notify_);
    session_->receive_msg();
    t_ = std::thread([this]{context_.run();});
  }

  return !ec;
}

void client::close()
{
  socket_.close();
  t_.join();
}

uint16_t client::send(const std::string& msg)
{
  return session_->send_msg(msg);
}
