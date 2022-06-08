// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Transport Layer.

#ifndef ASPS_MODBUS_PDU_SEMANTIC_SESSION_SERVICE_HPP
#define ASPS_MODBUS_PDU_SEMANTIC_SESSION_SERVICE_HPP

#include <cstdint>
#include <memory>
#include <list>
#include <asps/utility/utility.h>
#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>
#include <asps/modbus/pdu/semantic/request_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

using namespace asps::utility;

// Client Session Service
class client_observer
{
public:
  virtual ~client_observer() {}

public:
  virtual void update_send(const buffer_type& pdu) = 0;
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) = 0;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) = 0;
};

/*
 * Session service classe are also a subject
 */
class client_session_service
{
public:
  typedef std::shared_ptr<client_session_service> pointer_type;

public:
  virtual ~client_session_service() {}

public:
  virtual void receive_response(const buffer_type& pdu) = 0;
  virtual void send_request(request::pointer_type req) = 0;

public:
  virtual void register_observer(client_observer* o) {os_.push_back(o);}
  virtual void notify_send(const buffer_type& buffer)
  {
    for (auto o : os_) {
      o->update_send(buffer);
    }
  }
  virtual void notify_datas(const request::pointer_type& req,
                            const mb_datas& datas)
  {
    for (auto o : os_) {
      o->update_datas(req, datas);
    }
  }
  virtual void notify_exception(const request::pointer_type& req,
                                exception_code ec)
  {
    for (auto o : os_) {
      o->update_exception(req, ec);
    }
  }

protected:
  std::list<client_observer*> os_;
};

client_session_service::pointer_type
make_client_session();

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEMANTIC_SESSION_SERVICE_HPP
