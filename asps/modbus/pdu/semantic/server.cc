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
void
server::on_accept(const std::string& host, uint16_t port)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    event->on_accept(host, port);
  }
  // create session
  sessions_[address(host, port)] = std::make_shared<server_session>();
  if (transport_layer_) {
    transport_layer_->glance(host, port, server_session::mbap_header_size());
  }
}

void
server::on_error(const std::string& host, uint16_t port, const std::string& msg)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    event->on_error(msg);
  }
  // delete session
  sessions_.erase(address(host, port));
}

void
server::on_eof(const std::string& host, uint16_t port)
{
  // delete session
  sessions_.erase(address(host, port));
}

void
server::on_read(const std::string& host, uint16_t port, const uint8_t* buffer)
{
  //get session
  server_session::pointer_type session = sessions_[address(host, port)];
  buffer_list buffers = session->receive_request(buffer);

  if (transport_layer_) {
    for (auto buf : buffers) {
      transport_layer_->write(host, port, buf.first, buf.second);
    }

    transport_layer_->glance(host, port, server_session::mbap_header_size());
  }
}

void
server::on_glance(const std::string& host, uint16_t port, const uint8_t* buffer)
{
  // get session
  if (transport_layer_) {
    transport_layer_->read(host, port, server_session::adu_size(buffer));
  }
}

void
server::listen()
{
  if (transport_layer_) {
    transport_layer_->listen();
  }
}

void
server::transport_layer(server_transport_layer* layer)
{
  transport_layer_ = layer;
}

void
server::event(server_event* e)
{
  global_server_event::instance()->event(e);
}

void
server::run()
{
  if (transport_layer_) {
    transport_layer_->run();
  }
}
