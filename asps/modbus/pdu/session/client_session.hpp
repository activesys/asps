// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_PDU_SESSION_CLIENT_SESSION_HPP
#define ASPS_MODBUS_PDU_SESSION_CLIENT_SESSION_HPP

#include <queue>
#include <asps/modbus/pdu/semantic/request_service.hpp>
#include <asps/modbus/pdu/sequence/sequence_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Client Session Service
class client_session_observer
{
public:
  virtual ~client_session_observer() {}

public:
  virtual void update_send(const buffer_type& pdu) = 0;
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) = 0;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) = 0;
};

// Modbus client session
class client_session
  : public active_sequence_observer
{
public:
  void receive_response(const buffer_type& pdu);
  void send_request(request::pointer_type req);

public:
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) override;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) override;

private:
  void send_one_request();

public:
  virtual void register_observer(client_session_observer* obj)
  {csobjs_.push_back(obj);}
  virtual void notify_send(const buffer_type& buffer)
  {
    for (auto obj : csobjs_) {
      obj->update_send(buffer);
    }
  }
  virtual void notify_datas(const request::pointer_type& req,
                            const mb_datas& datas)
  {
    for (auto obj : csobjs_) {
      obj->update_datas(req, datas);
    }
  }
  virtual void notify_exception(const request::pointer_type& req,
                                exception_code ec)
  {
    for (auto obj : csobjs_) {
      obj->update_exception(req, ec);
    }
  }

private:
  active_sequence_service::pointer_type seq_;
  std::queue<request::pointer_type> reqs_;
  std::list<client_session_observer*> csobjs_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SESSION_CLIENT_SESSION_HPP
