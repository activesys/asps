// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#ifndef ASPS_DEMO_SESSION_DEMO_SESSION_H
#define ASPS_DEMO_SESSION_DEMO_SESSION_H

#include <cstdint>
#include <vector>

#include <asps/demo/session/message_service.h>
#include <asps/demo/session/timer_service.h>
#include <asps/demo/api/session_service.h>

namespace asps {
namespace demo {

// Demo Session
class demo_session : public session_service
{
public:
  demo_session(bool same_type,
               bool key_sequence,
               bool same_timestamp)
    : session_service(),
      same_type_(same_type),
      key_sequence_(key_sequence),
      same_timestamp_(same_timestamp),
      t1_(make_timer_service())
  {
    t1_->start(std::chrono::seconds(30),
               std::bind(&demo_session::t1_timeout, this));
  }
  ~demo_session()
  {
    t1_->stop();
  }

public:
  void same_type(bool flag) override {same_type_ = flag;}
  void key_sequence(bool flag) override {key_sequence_ = flag;}
  void same_timestamp(bool flag) override {same_timestamp_ = flag;}

  buffer_type& serialize_datas(const data_group_type& group) override
  {
    buffer_ = make_message_serialization_service(group,
                                                 same_type_,
                                                 key_sequence_,
                                                 same_timestamp_)->serialize();
    return buffer_;
  }

  buffer_type& serialize_keepalive() override
  {
    buffer_ = make_message_serialization_service()->serialize();
    return buffer_;
  }

  bool receive(const uint8_t* buffer) override
  {
    t2_->stop();
    return make_message_unserialization_service(0xff)->unserialize(buffer);
  }

  void t1_timeout()
  {
    notify_positive_keepalive();
    t2_ = make_timer_service();
    t2_->start(std::chrono::seconds(20), std::bind(&demo_session::t2_timeout, this));
  }

  void t2_timeout()
  {
    notify_missing_positive_keepalive_ack();
  }

private:
  bool same_type_;
  bool key_sequence_;
  bool same_timestamp_;
  buffer_type buffer_;
  timer_service::pointer_type t1_;
  timer_service::pointer_type t2_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_DEMO_SESSION_H
