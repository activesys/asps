// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#include <memory>
#include <asps/demo/session/demo_session.h>
#include <asps/demo/utility/config.h>

namespace asps {
namespace demo {

demo_session::demo_session()
  : session_service(),
    t1_(make_timer_service())
{
  t1_->start(std::chrono::seconds(config::t1()),
             std::bind(&demo_session::t1_timeout, this));
}

demo_session::~demo_session()
{
  t1_->stop();
}

demo_session::buffer_type&
demo_session::serialize_datas(const data_group_type& group)
{
  buffer_ = make_message_serialization_service(group)->serialize();
  return buffer_;
}

demo_session::buffer_type&
demo_session::serialize_keepalive()
{
  buffer_ = make_message_serialization_service()->serialize();
  return buffer_;
}

bool demo_session::receive(const uint8_t* buffer)
{
  t2_->stop();
  return make_message_unserialization_service(
          config::positive_keepalive_ack())->unserialize(buffer);
}

void demo_session::t1_timeout()
{
  notify_positive_keepalive();
  t2_ = make_timer_service();
  t2_->start(std::chrono::seconds(config::t2()),
             std::bind(&demo_session::t2_timeout, this));
}

void demo_session::t2_timeout()
{
  notify_missing_positive_keepalive_ack();
}

session_service::pointer_type make_session_service()
{
  return std::make_shared<demo_session>();
}

} // demo
} // asps

