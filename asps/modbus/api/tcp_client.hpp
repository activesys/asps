// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP Client.

#ifndef ASPS_MODBUS_API_TCP_CLIENT_HPP
#define ASPS_MODBUS_API_TCP_CLIENT_HPP

#include <string>
#include <functional>
#include <asps/modbus/pdu/semantic/pdu_client.hpp>
#include <asps/modbus/adu/semantic/adu_client.hpp>
#include <asps/transport/transport_service.hpp>
#include <asps/utility/timer_service.hpp>

namespace asps {
namespace modbus {

namespace tp = asps::transport;
namespace ut = asps::utility;
using namespace std::placeholders;

// Asynce Modbus TCP Client
class async_tcp_client
{
public:
  async_tcp_client(const std::string& ip, uint16_t port)
    : connector_(tp::make_connector(ip, port))
  {
    connector_->set_connect_handler(std::bind(&async_tcp_client::connect_handler,
                                              this,
                                              _1));
  }

public:
  void connect(uint32_t timeout);
  void close();
  void stop();
  void run();

public:
  void read_coils(uint16_t starting_address,
                  uint16_t quantity_of_coils);
  void read_discrete_inputs(uint16_t starting_address,
                            uint16_t quantity_of_inputs);
  void read_holding_registers(uint16_t starting_address,
                              uint16_t quantity_of_registers);
  void read_input_registers(uint16_t starting_address,
                            uint16_t quantity_of_registers);

public:
  virtual void on_connect() {}
  virtual void on_read_coils(const pdu::coils& status) {}
  virtual void on_read_discrete_inputs(const pdu::discrete_inputs& status) {}
  virtual void on_read_holding_registers(const pdu::holding_registers& registers) {}
  virtual void on_read_input_registers(const pdu::input_registers& registers) {}

private:
  void connect_handler(tp::connection::pointer_type conn);
  void timeout_handler();
  void read_coils_handler(const pdu::coils& status);
  void read_discrete_inputs_handler(const pdu::discrete_inputs& status);
  void read_holding_registers_handler(const pdu::holding_registers& registers);
  void read_input_registers_handler(const pdu::input_registers& registers);

private:
  tp::connector::pointer_type     connector_;
  tp::connection::pointer_type    connection_;
  pdu::pdu_client::pointer_type   pdu_;
  adu::adu_client::pointer_type   adu_;
  ut::timer_service::pointer_type timer_;
};

} // modbus
} // asps

#endif // ASPS_MODBUS_API_TCP_CLIENT_HPP
