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
void
client::on_connect(const std::string& address, uint16_t port)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    event->on_connect(address, port);
  }
}

void
client::on_error(const std::string& error_message)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    event->on_error(error_message);
  }
}

void
client::on_eof()
{
  if (transport_layer_) {
    transport_layer_->connect();
  }
}

void
client::on_glance(const uint8_t* buffer)
{
  if (transport_layer_) {
    transport_layer_->read(client_session::adu_size(buffer));
  }
}

void
client::on_read(const uint8_t* buffer)
{
  buffer_list&& buffers = session_.receive_response(buffer);
  if (transport_layer_) {
    for (auto& buf : buffers) {
      transport_layer_->write(buf.first, buf.second);
    }
  }
}

void
client::connect()
{
  if (transport_layer_) {
    transport_layer_->connect();
  }
}

void
client::read_coils(const coils& cs)
{
  buffer_list&& buffers = session_.read_coils(cs);
  if (transport_layer_) {
    for (auto& buf : buffers) {
      transport_layer_->write(buf.first, buf.second);
    }
  }
}

void
client::receive_response()
{
  if (transport_layer_) {
    transport_layer_->glance(client_session::mbap_header_size());
  }
}

void
client::transport_layer(client_transport_layer* layer)
{
  transport_layer_ = layer;
}

void
client::event(client_event* e)
{
  global_client_event::instance()->event(e);
}

void
client::run()
{
  if (transport_layer_) {
    transport_layer_->run();
  }
}

void
client::close()
{
  if (transport_layer_) {
    transport_layer_->close();
  }
}
