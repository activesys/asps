// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Client.

#include <asps/modbus/pdu/semantic/pdu_client.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Modbus PDU Client
void pdu_client::read_coils(uint16_t starting_address,
                            uint16_t quantity_of_coils)
{
  session_.send_request(make_read_coils_request(starting_address,
                                                quantity_of_coils));
}

void pdu_client::read_discrete_inputs(uint16_t starting_address,
                                      uint16_t quantity_of_inputs)
{
  session_.send_request(make_read_discrete_inputs_request(starting_address,
                                                          quantity_of_inputs));
}

void pdu_client::read_holding_registers(uint16_t starting_address,
                                        uint16_t quantity_of_registers)
{
  session_.send_request(make_read_holding_registers_request(starting_address,
                                                            quantity_of_registers));
}

void pdu_client::read_input_registers(uint16_t starting_address,
                                      uint16_t quantity_of_registers)
{
  session_.send_request(make_read_input_registers_request(starting_address,
                                                          quantity_of_registers));
}

void pdu_client::update_send(const buffer_type& pdu)
{
  adu_->write(pdu);
}

void pdu_client::update_datas(const request::pointer_type& req,
                              const mb_datas& datas)
{
  switch (req->func_code())
  {
  case function_code_read_coils:
    if (rchandler_) {
      rchandler_(dynamic_cast<const coils&>(datas));
    }
    break;

  case function_code_read_discrete_inputs:
    if (rdihandler_) {
      rdihandler_(dynamic_cast<const discrete_inputs&>(datas));
    }
    break;

  case function_code_read_holding_registers:
    if (rhrhandler_) {
      rhrhandler_(dynamic_cast<const holding_registers&>(datas));
    }
    break;

  case function_code_read_input_registers:
    if (rirhandler_) {
      rirhandler_(dynamic_cast<const input_registers&>(datas));
    }
    break;
  }
}

void pdu_client::update_exception(const request::pointer_type& req,
                                  exception_code ec)
{
  if (ehandler_) {
    ehandler_(req->func_code(), ec);
  }
}

void pdu_client::adu_read_handler(const buffer_type& pdu)
{
  session_.receive_response(pdu);
}

} // pdu
} // modbus
} // asps
