// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client.

#include <functional>
#include <asps/modbus/api/client.h>
#include <asps/modbus/common/global_event.h>

using namespace asps::modbus;
using namespace std::placeholders;

// Modbus Client
void client::on_connect(const std::string& address, uint16_t port)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    event->on_connect(address, port);
  }
}

void client::on_error(const std::string& error_message)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    event->on_error(error_message);
  }
}

void client::on_eof()
{
  transport_layer_.connect(
    std::bind(&client::on_connect, this, _1, _2),
    std::bind(&client::on_error, this, _1));
}

void client::on_glance(const uint8_t* buffer)
{
  transport_layer_.read(
    session_.adu_size(buffer),
    std::bind(&client::on_read, this, _1),
    std::bind(&client::on_eof, this),
    std::bind(&client::on_error, this, _1));
}

void client::on_read(const uint8_t* buffer)
{
  session_.receive_response(buffer);
}

void client::connect()
{
  transport_layer_.connect(
    std::bind(&client::on_connect, this, _1, _2),
    std::bind(&client::on_error, this, _1));
}

void client::read_coils(const coils& cs)
{
  session_.read_coils(cs);
}

void client::receive_response()
{
  transport_layer_.glance(
    session_.mbap_header_size(),
    std::bind(&client::on_glance, this, _1),
    std::bind(&client::on_eof, this),
    std::bind(&client::on_error, this, _1));
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
