// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#include <asps/modbus/pdu/session/client_session.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Client Session
void client_session::receive_response(const buffer_type& buffer)
{
  if (seq_->receive_response(buffer)) {
    seq_.reset();
    send_one_request();
  }
}

void client_session::send_request(request::pointer_type req)
{
  reqs_.push(req);
  send_one_request();
}

void client_session::update_datas(const request::pointer_type& req,
                                  const mb_datas& datas)
{
  notify_datas(req, datas);
}

void client_session::update_exception(const request::pointer_type& req,
                                      exception_code ec)
{}

void client_session::send_one_request()
{
  if (!seq_ && reqs_.size() && reqs_.front()->valid()) {
    seq_ = make_active_sequence(reqs_.front()->split());
    seq_->register_event_observer(this);
    notify_send(seq_->send_request());

    if (!reqs_.front()->valid()) {
      reqs_.pop();
    }
  }
}

} // pdu
} // modbus
} // asps
