// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_PDU_SESSION_SESSION_HPP
#define ASPS_MODBUS_PDU_SESSION_SESSION_HPP

#include <queue>
#include <asps/modbus/pdu/semantic/session_service.hpp>
#include <asps/modbus/pdu/semantic/request_service.hpp>
#include <asps/modbus/pdu/session/sequence_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Modbus client session
class client_session
  : public client_session_service,
    public active_observer
{
public:
  virtual void receive_response(const buffer_type& pdu) override;
  virtual void send_request(request::pointer_type req) override;

public:
  virtual void update_event() override;
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) override;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) override;

private:
  void send_one_request();

private:
  active_sequence_service::pointer_type seq_;
  std::queue<request::pointer_type> reqs_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SESSION_SESSION_HPP
