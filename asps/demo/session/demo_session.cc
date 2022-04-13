// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#include <memory>
#include <asps/demo/session/demo_session.h>
#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

demo_session::demo_session()
  : session_service(),
    pkeep_seq_(make_sequence_service(true)),
    t1_(make_timer_service())
{
  t1_->start(std::chrono::seconds(config::t1()),
             std::bind(&demo_session::t1_timeout, this));
}

demo_session::~demo_session()
{
  t1_->stop();
}

bool demo_session::send(const data_group_type& group)
{
  sequence_service::pointer_type seq = make_sequence_service(group);
  const buffer_type& buffer = seq->request();

  notify_send(buffer);

  return true;
}

bool demo_session::receive(const uint8_t* buffer)
{
  sequence_type type = get_sequence_type(buffer);

  if (type == belong_to_positive_keepalive_sequence) {

    return pkeep_seq_->response(buffer);

  } else if (type == belong_to_negative_keepalive_sequence) {

    sequence_service::pointer_type seq = make_sequence_service(false);
    if (!seq->response(buffer)) {
      return false;
    }

    const buffer_type& buf = seq->request();
    notify_send(buf);

    return true;

  } else {

    return false;

  }
}

void demo_session::t1_timeout()
{
  const buffer_type& buf = pkeep_seq_->request();
  notify_send(buf);
}

void demo_session::update_event()
{
  notify_event();
}

session_service::pointer_type make_session_service()
{
  return std::make_shared<demo_session>();
}

} // demo
} // asps

