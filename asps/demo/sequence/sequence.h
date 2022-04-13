// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Sequence.

#ifndef ASPS_DEMO_SEQUENCE_SEQUENCE_H
#define ASPS_DEMO_SEQUENCE_SEQUENCE_H

#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/session/timer_service.h>
#include <asps/demo/session/sequence_service.h>
#include <asps/demo/sequence/message_service.h>
#include <asps/demo/sequence/state.h>

namespace asps {
namespace demo {

class data_sequence : public sequence_service
{
public:
  data_sequence(const data_group_type& group);
  virtual ~data_sequence() {};

public:
  const buffer_type& request() override;
  bool response(const uint8_t* buffer) override;

private:
  message_serialization_service::pointer_type message_;
};

class positive_keepalive_sequence : public sequence_service
{
public:
  positive_keepalive_sequence();
  virtual ~positive_keepalive_sequence() {}

public:
  const buffer_type& request() override;
  bool response(const uint8_t* buffer) override;

public:
  const buffer_type& serialize();
  bool unserialize(const uint8_t* buffer);
  void t2_timeout();
  void t2_start();
  void t2_stop();
  void change_state(state* s);

private:
  friend class state;
  state* state_;

private:
  message_serialization_service::pointer_type keepalive_;
  message_unserialization_service::pointer_type ack_;
  timer_service::pointer_type t2_;
};

class negative_keepalive_sequence : public sequence_service
{
public:
  negative_keepalive_sequence();
  virtual ~negative_keepalive_sequence() {}

public:
  const buffer_type& request() override;
  bool response(const uint8_t* buffer) override;

private:
  message_unserialization_service::pointer_type keepalive_;
  message_serialization_service::pointer_type ack_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_SEQUENCE_H
