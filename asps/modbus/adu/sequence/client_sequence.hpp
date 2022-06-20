// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Sequence.

#ifndef ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP
#define ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP

#include <list>
#include <memory>
#include <asps/modbus/adu/sequence/state.hpp>
#include <asps/modbus/adu/message/client_message.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Client Sequence Observer
class client_sequence_observer
{
public:
  virtual ~client_sequence_observer() {}

public:
  virtual void update_pdu(const buffer_type& pdu) = 0;
};

// Client Sequence
class client_sequence
{
public:
  typedef std::shared_ptr<client_sequence> pointer_type;

public:
  client_sequence(uint16_t tid, uint8_t uid, const buffer_type& pdu)
    : state_(idle_state::instance()),
      request_(tid, uid, pdu)
  {}
  virtual ~client_sequence() {}

public:
  const buffer_type& send_request();
  bool receive_response(const buffer_type& adu);

public:
  const buffer_type& serialize();
  bool unserialize(const buffer_type& buffer);
  void notify();
  void change_state(state* s);

public:
  virtual void register_event_observer(client_sequence_observer* obj)
  {csobjs_.push_back(obj);}
  virtual void notify_pdu(const buffer_type& pdu)
  {
    for (auto obj : csobjs_) {
      obj->update_pdu(pdu);
    }
  }

private:
  state* state_;

private:
  client_request_adu request_;
  client_response_adu response_;
  std::list<client_sequence_observer*> csobjs_;
};

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP
