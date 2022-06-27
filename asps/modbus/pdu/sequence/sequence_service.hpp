// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Active Sequence Interface.

#ifndef ASPS_MODBUS_PDU_SEQUENCE_SEQUENCE_SERVICE_HPP
#define ASPS_MODBUS_PDU_SEQUENCE_SEQUENCE_SERVICE_HPP

#include <memory>
#include <list>
#include <asps/utility/utility.hpp>
#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>
#include <asps/modbus/pdu/semantic/request_service.hpp>
#include <asps/modbus/pdu/message/message_service.hpp>
#include <asps/modbus/pdu/sequence/state.hpp>

namespace asps {
namespace modbus {
namespace pdu {

using namespace asps::utility;

// Active Sequence : send request and receive response
class active_sequence_observer
{
public:
  virtual ~active_sequence_observer() {}

public:
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
  active_sequence_service(const request::pointer_type& req);
  virtual ~active_sequence_service() {}

public:
  virtual const buffer_type& send_request();
  virtual bool receive_response(const buffer_type& buffer);

public:
  virtual void register_event_observer(active_sequence_observer* obj)
  {asobjs_.push_back(obj);}
  virtual void notify_datas(const mb_datas& datas)
  {
    for (auto obj : asobjs_) {
      obj->update_datas(req_, datas);
    }
  }
  virtual void notify_exception(exception_code ec)
  {
    for (auto obj : asobjs_) {
      obj->update_exception(req_, ec);
    }
  }

public:
  const buffer_type& serialize();
  bool unserialize(const buffer_type& buffer);
  void notify();
  void change_state(state* s);

protected:
  virtual void sync_response() {}

private:
  state* state_;

protected:
  request::pointer_type req_;
  message_serialization_service::pointer_type request_;
  message_unserialization_service::pointer_type response_;
  std::list<active_sequence_observer*> asobjs_;
};

active_sequence_service::pointer_type
make_active_sequence(request::pointer_type req);

// Passive Sequence
/*
class passive_sequence
  : public passive_sequence_service
{
public:
  passive_sequence();
  virtual ~passive_sequence() {}

public:
  const buffer_type& send_response() override;
  bool receive_request(buffer_type& buffer) override;

protected:
  message_unserialization_service::pointer_type keepalive_;
  message_serialization_service::pointer_type ack_;
};
*/

// Garbage Collector Sequnece
/*
class garbage_collector_sequence
  : public garbage_collector_sequence_service
{
public:
  garbage_collector_sequence();
  virtual ~garbage_collector_sequence() {}

public:
  bool clear(buffer_type& buffer) override;

private:
  message_unserialization_service::pointer_type message_;
};
*/

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEQUENCE_SEQUENCE_SERVICE_HPP
