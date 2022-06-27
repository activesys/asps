// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP Client.

#include <asps/modbus/api/tcp_client.hpp>

namespace asps {
namespace modbus {

// Async Modbus TCP Client
void async_tcp_client::connect(uint32_t timeout)
{
  connector_->connect();
  if (timeout > 0) {
    timer_ = ut::make_timer(timeout * 1000,
                            std::bind(&async_tcp_client::timeout_handler, this),
                            true);
    timer_->start();
  }
}

void async_tcp_client::close()
{
  if (connection_) {
    connection_->close();
  }
}

void async_tcp_client::stop()
{
  connector_->stop();
}

void async_tcp_client::run()
{
  connector_->run();
}

void async_tcp_client::read_coils(uint16_t starting_address,
                                  uint16_t quantity_of_coils)
{
  if (pdu_) {
    pdu_->read_coils(starting_address, quantity_of_coils);
    connection_->read();
  }
}

void async_tcp_client::read_discrete_inputs(uint16_t starting_address,
                                            uint16_t quantity_of_inputs)
{
  if (pdu_) {
    pdu_->read_discrete_inputs(starting_address, quantity_of_inputs);
    connection_->read();
  }
}

void async_tcp_client::connect_handler(tp::connection::pointer_type conn)
{
  timer_->stop();

  connection_ = conn;
  adu_ = std::make_shared<adu::adu_client>(conn);
  pdu_ = std::make_shared<pdu::pdu_client>(adu_);

  pdu_->set_read_coils_handler(std::bind(&async_tcp_client::read_coils_handler,
                                         this,
                                         _1));
  pdu_->set_read_discrete_inputs_handler(std::bind(&async_tcp_client::read_discrete_inputs_handler,
                                                   this,
                                                   _1));

  on_connect();
}

void async_tcp_client::timeout_handler()
{
  connector_->connect();
}

void async_tcp_client::read_coils_handler(const pdu::coils& status)
{
  on_read_coils(status);
}

void async_tcp_client::read_discrete_inputs_handler(const pdu::discrete_inputs& status)
{
  on_read_discrete_inputs(status);
}

} // modbus
} // asps
