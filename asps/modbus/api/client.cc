// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client.

#include <asps/modbus/api/client.h>
#include <asps/modbus/common/global_event.h>

using namespace asps::modbus;

// Modbus Client
void client::read_coils(const coils& cs)
{
  session_.read_coils(cs);
}

void client::receive_response()
{
  transport_layer_.read(
    session_.adu_size(
      transport_layer_.glance(
        session_.mbap_header_size())));
}

void client::event(client_event* e)
{
  global_client_event::instance()->event(e);
}
/*
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
        session_ = std::make_shared<client_session>(1, transport_layer_, event_);
        //session_->start();

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

void client::run()
{
  context_.run();
}

void client::read_coils(uint8_t unit_identifier, const coils& cs)
{
  session_->read_coils(cs);
}
*/
