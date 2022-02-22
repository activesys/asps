// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#include <memory>
#include <asps/modbus/pdu/sequence/pdu_sequence.h>

using namespace asps::modbus;

// Modbus read coils pdu client sequence
mb_pdu::pointer_type read_coils_pdu_client_sequence::get_request()
{
  return std::make_shared<read_coils_request>(
            coils_->starting_address(), coils_->count());
}

void read_coils_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (read_coils_response* p = dynamic_cast<read_coils_response*>(pdu.get())) {
      for (uint16_t i = 0; i < coils_->count(); ++i) {
        coils_->at(coils_->starting_address() + i) = p->status().at(i);
      }
      event_->on_read_coils(coils_, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event_->on_read_coils(coils_, e->code());
    } else {
      event_->on_error("Invalid Read Coils Response PDU");
    }
  }
}

// Modbus write single coil pdu client sequence
mb_pdu::pointer_type write_single_coil_pdu_client_sequence::get_request()
{
  return std::make_shared<write_single_coil_request>(
             coils_->starting_address(),
             coils_->at(coils_->starting_address()));
}

void write_single_coil_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (write_single_coil_response* p =
          dynamic_cast<write_single_coil_response*>(pdu.get())) {
      coils_->at(coils_->starting_address()) = p->output_value();
      event_->on_write_single_coil(coils_, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event_->on_write_single_coil(coils_, e->code());
    } else {
      event_->on_error("Invalid Write Single Coil Response PDU");
    }
  }
}

// Modbus write multiple coils pdu client sequence
mb_pdu::pointer_type write_multiple_coils_pdu_client_sequence::get_request()
{
  mb_pdu::coils_type ct(coils_->count());
  for (std::size_t i = 0; i < coils_->count(); ++i) {
    ct[i] = coils_->at(coils_->starting_address() + i);
  }
  return std::make_shared<write_multiple_coils_request>(
          coils_->starting_address(), coils_->count(), ct);
}

void
write_multiple_coils_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (write_multiple_coils_response* p =
          dynamic_cast<write_multiple_coils_response*>(pdu.get())) {
      event_->on_write_multiple_coils(coils_, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event_->on_write_multiple_coils(coils_, e->code());
    } else {
      event_->on_error("Invalid Write Multiple Coils Response PDU");
    }
  }
}

// Modbus invalid pdu client sequence
mb_pdu::pointer_type invalid_pdu_client_sequence::get_request()
{
  return std::make_shared<excep_pdu>(invalid_pdu, illegal_function);
}

void invalid_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  if (event_) {
    event_->on_error("Invalid Excep PDU");
  }
}

// Modbus read coils pdu server sequence
mb_pdu::pointer_type
read_coils_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (read_coils_request* p = dynamic_cast<read_coils_request*>(pdu.get())) {
      bool status[65535];
      const coils::ptr_type req_cs = std::make_shared<coils>(
        p->starting_address(), p->quantity_of_coils(), status);
      const coils::ptr_type rsp_cs = event_->on_read_coils(req_cs);
      if (rsp_cs->code() == success) {
        mb_pdu::coils_type status(rsp_cs->count());
        for (uint16_t i = 0; i < rsp_cs->count(); ++i) {
          status[i] = rsp_cs->at(rsp_cs->starting_address() + i);
        }
        return std::make_shared<read_coils_response>(status);
      } else {
        return std::make_shared<excep_pdu>(read_coils, rsp_cs->code());
      }
    } else {
      event_->on_error("Invalid Read Coils Request PDU");
      return std::make_shared<excep_pdu>(read_coils, server_device_failure);
    }
  }

  return std::make_shared<excep_pdu>(read_coils, server_device_failure);
}

// Modbus write single coil pdu server sequence
mb_pdu::pointer_type
write_single_coil_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (write_single_coil_request* p =
          dynamic_cast<write_single_coil_request*>(pdu.get())) {
      bool status[65535]{p->output_value()};
      const coils::ptr_type req_cs = std::make_shared<coils>(
        p->output_address(), 1, status);
      const coils::ptr_type rsp_cs = event_->on_write_single_coil(req_cs);
      if (rsp_cs->code() == success) {
        return std::make_shared<write_single_coil_response>(
                p->output_address(), p->output_value());
      } else {
        return std::make_shared<excep_pdu>(write_single_coil, rsp_cs->code());
      }
    } else {
      event_->on_error("Invalid Write Single Coil Request PDU");
      return std::make_shared<excep_pdu>(
              write_single_coil, server_device_failure);
    }
  }

  return std::make_shared<excep_pdu>(write_single_coil, server_device_failure);
}

// Modbus write multiple coils pdu server sequence
mb_pdu::pointer_type
write_multiple_coils_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  if (event_) {
    if (write_multiple_coils_request* p =
        dynamic_cast<write_multiple_coils_request*>(pdu.get())) {
      bool status[65535];
      for (std::size_t i = 0; i < p->quantity_of_outputs(); ++i) {
        status[i] = p->outputs_value()[i];
      }
      const coils::ptr_type req_cs = std::make_shared<coils>(
        p->starting_address(), p->quantity_of_outputs(), status);
      const coils::ptr_type rsp_cs = event_->on_write_multiple_coils(req_cs);
      if (rsp_cs->code() == success) {
        return std::make_shared<write_multiple_coils_response>(
                p->starting_address(), p->quantity_of_outputs());
      } else {
        return std::make_shared<excep_pdu>(write_multiple_coils, rsp_cs->code());
      }
    } else {
      event_->on_error("Invalid Write Multiple Coils Request PDU");
      return std::make_shared<excep_pdu>(
              write_multiple_coils, server_device_failure);
    }
  }

  return std::make_shared<excep_pdu>(
          write_multiple_coils, server_device_failure);
}

// Modbus Invalid PDU Server Sequence
mb_pdu::pointer_type
invalid_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  return std::make_shared<excep_pdu>(invalid_pdu, server_device_failure);
}
