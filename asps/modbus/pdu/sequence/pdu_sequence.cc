// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#include <memory>
#include <asps/modbus/common/global_event.h>
#include <asps/modbus/pdu/sequence/pdu_sequence.h>

using namespace asps::modbus;

// Modbus read coils pdu client sequence
mb_pdu::pointer_type read_coils_pdu_client_sequence::get_request()
{
  req_ = std::make_shared<read_coils_request>(
          coils_->starting_address(), coils_->count());
  return req_;
}

void read_coils_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    read_coils_response* rsp = dynamic_cast<read_coils_response*>(pdu.get());
    read_coils_request* req = dynamic_cast<read_coils_request*>(req_.get());

    if (req && rsp) {
      coils::pointer_type cs = std::make_shared<coils>(
        req->starting_address(), req->quantity_of_coils(), rsp->status());

      event->on_read_coils(cs, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event->on_read_coils(coils_, e->code());
    } else {
      event->on_error("Invalid Read Coils Response PDU");
    }
  }
}

// Modbus write single coil pdu client sequence
mb_pdu::pointer_type write_single_coil_pdu_client_sequence::get_request()
{
  req_ = std::make_shared<write_single_coil_request>(
             coils_->starting_address(),
             coils_->bit(coils_->starting_address()));
  return req_;
}

void write_single_coil_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    write_single_coil_request* req =
      dynamic_cast<write_single_coil_request*>(req_.get());
    write_single_coil_response* rsp =
      dynamic_cast<write_single_coil_response*>(pdu.get());

    if (req && rsp) {
      coils::pointer_type cs = std::make_shared<coils>(
        req->output_address(), 1, coils_->status());

      cs->bit(req->output_address(), req->output_value());
      event->on_write_single_coil(cs, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event->on_write_single_coil(coils_, e->code());
    } else {
      event->on_error("Invalid Write Single Coil Response PDU");
    }
  }
}

// Modbus write multiple coils pdu client sequence
mb_pdu::pointer_type write_multiple_coils_pdu_client_sequence::get_request()
{
  req_ = std::make_shared<write_multiple_coils_request>(
          coils_->starting_address(), coils_->count(), coils_->status());
  return req_;
}

void
write_multiple_coils_pdu_client_sequence::set_response(mb_pdu::pointer_type pdu)
{
  client_event* event = global_client_event::instance()->event();
  if (event) {
    write_multiple_coils_request* req =
      dynamic_cast<write_multiple_coils_request*>(req_.get());
    write_multiple_coils_response* rsp =
      dynamic_cast<write_multiple_coils_response*>(pdu.get());

    if (req && rsp) {
      coils::pointer_type cs = std::make_shared<coils>(
        req->starting_address(), req->quantity_of_outputs(), req->outputs_value());

      event->on_write_multiple_coils(cs, success);
    } else if (excep_pdu* e = dynamic_cast<excep_pdu*>(pdu.get())) {
      event->on_write_multiple_coils(coils_, e->code());
    } else {
      event->on_error("Invalid Write Multiple Coils Response PDU");
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
  client_event* event = global_client_event::instance()->event();
  if (event) {
    event->on_error("Invalid Excep PDU");
  }
}

// Modbus read coils pdu server sequence
mb_pdu::pointer_type
read_coils_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    read_coils_request* req = dynamic_cast<read_coils_request*>(pdu.get());
    if (req) {
      const coils::pointer_type req_cs = std::make_shared<coils>(
        req->starting_address(), req->quantity_of_coils());
      const coils::pointer_type rsp_cs = event->on_read_coils(req_cs);

      if (rsp_cs->code() == success) {
        return std::make_shared<read_coils_response>(rsp_cs->status());
      } else {
        return std::make_shared<excep_pdu>(read_coils, rsp_cs->code());
      }
    } else {
      event->on_error("Invalid Read Coils Request PDU");
      return std::make_shared<excep_pdu>(read_coils, server_device_failure);
    }
  }

  return std::make_shared<excep_pdu>(read_coils, server_device_failure);
}

// Modbus write single coil pdu server sequence
mb_pdu::pointer_type
write_single_coil_pdu_server_sequence::set_request(mb_pdu::pointer_type pdu)
{
  server_event* event = global_server_event::instance()->event();
  if (event) {
    write_single_coil_request* req =
      dynamic_cast<write_single_coil_request*>(pdu.get());

    if (req) {
      const coils::pointer_type req_cs =
        std::make_shared<coils>(req->output_address(), 1);
      req_cs->bit(req_cs->starting_address(), req->output_value());
      const coils::pointer_type rsp_cs = event->on_write_single_coil(req_cs);

      if (rsp_cs->code() == success) {
        return std::make_shared<write_single_coil_response>(
                rsp_cs->starting_address(),
                rsp_cs->bit(rsp_cs->starting_address()));
      } else {
        return std::make_shared<excep_pdu>(write_single_coil, rsp_cs->code());
      }
    } else {
      event->on_error("Invalid Write Single Coil Request PDU");
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
  server_event* event = global_server_event::instance()->event();
  if (event) {
    write_multiple_coils_request* req =
      dynamic_cast<write_multiple_coils_request*>(pdu.get());

    if (req) {
      const coils::pointer_type req_cs =
        std::make_shared<coils>(
          req->starting_address(),
          req->quantity_of_outputs(),
          req->outputs_value());
      const coils::pointer_type rsp_cs = event->on_write_multiple_coils(req_cs);

      if (rsp_cs->code() == success) {
        return std::make_shared<write_multiple_coils_response>(
                rsp_cs->starting_address(), rsp_cs->count());
      } else {
        return std::make_shared<excep_pdu>(write_multiple_coils, rsp_cs->code());
      }
    } else {
      event->on_error("Invalid Write Multiple Coils Request PDU");
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
