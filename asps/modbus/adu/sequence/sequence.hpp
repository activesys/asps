// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Sequence.

#ifndef ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP
#define ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP

#include <asps/modbus/adu/session/sequence_service.hpp>
#include <asps/modbus/adu/sequence/state.hpp>
#include <asps/modbus/adu/sequence/message_service.hpp>
#include <asps/utility/timer_service.h>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::utility;

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

// Active Sequence
class active_sequence
  : public active_sequence_service
{
public:
  active_sequence();
  virtual ~active_sequence() {}

public:
  const buffer_type& send_request() override;
  bool receive_response(const buffer_type& adu) override;

public:
  const buffer_type& serialize();
  bool unserialize(const buffer_type& buffer);
  void notify();
  //void t2_timeout();
  //void t2_start();
  //void t2_stop();
  void change_state(state* s);

private:
  state* state_;
  //timer_service::pointer_type t2_;

protected:
  message_serialization_service::pointer_type request_;
  message_unserialization_service::pointer_type response_;
};

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

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEQUENCE_SEQUENCE_HPP
