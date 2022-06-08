// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Active Sequence Interface.

#ifndef ASPS_MODBUS_PDU_SESSION_SEQUENCE_SERVICE_HPP
#define ASPS_MODBUS_PDU_SESSION_SEQUENCE_SERVICE_HPP

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

// Active Sequence : send request and receive response
class active_observer
{
public:
  virtual ~active_observer() {}

public:
  virtual void update_event() = 0;
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) = 0;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) = 0;
};

class active_sequence_service
{
public:
  typedef std::shared_ptr<active_sequence_service> pointer_type;

public:
  active_sequence_service(const request::pointer_type& req)
    : req_(req)
  {}
  virtual ~active_sequence_service() {}

public:
  virtual const buffer_type& send_request() = 0;
  virtual bool receive_response(const buffer_type& buffer) = 0;

public:
  virtual void register_event_observer(active_observer* o) {aos_.push_back(o);}
  virtual void notify_event()
  {
    for (auto o : aos_) {
      o->update_event();
    }
  }
  virtual void notify_datas(const mb_datas& datas)
  {
    for (auto o : aos_) {
      o->update_datas(req_, datas);
    }
  }
  virtual void notify_exception(exception_code ec)
  {
    for (auto o : aos_) {
      o->update_exception(req_, ec);
    }
  }

protected:
  std::list<active_observer*> aos_;
  request::pointer_type req_;
};

active_sequence_service::pointer_type
make_active_sequence(request::pointer_type req);

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SESSION_SEQUENCE_SERVICE_HPP
