// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Sequence.

#ifndef ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H
#define ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H

#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/utility/timer_service.h>
#include <asps/demo/session/sequence_service.h>
#include <asps/demo/sequence/message_service.h>
#include <asps/demo/sequence/state.h>

namespace asps {
namespace demo {

// Client Data Sequence
class client_data_sequence
  : public client_data_sequence_service
{
public:
  const buffer_type& request(const data_group_type& group) override;

private:
  message_serialization_service::pointer_type message_;
};

// Client Positive Keepalive Sequence
class client_positive_keepalive_sequence
  : public client_positive_keepalive_sequence_service
{
public:
  client_positive_keepalive_sequence();

public:
  const buffer_type& request() override;
  bool response(buffer_type& buffer) override;

public:
  const buffer_type& serialize();
  bool unserialize(buffer_type& buffer);
  void t2_timeout();
  void t2_start();
  void t2_stop();
  void change_state(state* s);

private:
  state* state_;

private:
  message_serialization_service::pointer_type keepalive_;
  message_unserialization_service::pointer_type ack_;
  timer_service::pointer_type t2_;
};

// Client Negative Keepalive Sequence
class client_negative_keepalive_sequence
  : public client_negative_keepalive_sequence_service
{
public:
  client_negative_keepalive_sequence();
  virtual ~client_negative_keepalive_sequence() {}

public:
  const buffer_type& request() override;
  bool response(buffer_type& buffer) override;

private:
  message_unserialization_service::pointer_type keepalive_;
  message_serialization_service::pointer_type ack_;
};

// Client Invalid Sequnece
class client_invalid_sequence
  : public client_invalid_sequence_service
{
public:
  client_invalid_sequence();
  virtual ~client_invalid_sequence() {}

public:
  bool response(buffer_type& buffer) override;

private:
  message_unserialization_service::pointer_type message_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H
