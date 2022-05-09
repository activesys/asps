// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Session.

#include <memory>
#include <asps/demo/session/client_session.h>
#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

client_session::client_session()
  : client_session_service(),
    pkeep_sequence_(make_active_sequence(false)),
    data_sequence_(make_data_send_sequence()),
    gc_sequence_(make_garbage_collector_sequence()),
    t1_(make_timer_service(config::t1(),
                           std::bind(&client_session::t1_timeout, this)))
{
  t1_->start();
  pkeep_sequence_->register_event_observer(this);
}

client_session::~client_session()
{
  t1_->stop();
}

bool client_session::send(const data_group_type& group)
{
  const buffer_type& buffer = data_sequence_->send_data(group);

  notify_send(buffer);

  return true;
}

void client_session::receive(buffer_type& buffer)
{
  while (receive_one_package(buffer));
}

bool client_session::receive_one_package(buffer_type& buffer)
{
  sequence_type type = get_client_sequence_type(buffer);

  if (type == belong_to_positive_keepalive_sequence) {

    return pkeep_sequence_->receive_response(buffer);

  } else if (type == belong_to_negative_keepalive_sequence) {

    passive_sequence_service::pointer_type seq = make_passive_sequence(false);
    if (!seq->receive_request(buffer)) {
      return false;
    }

    const buffer_type& buf = seq->send_response();
    notify_send(buf);

    return true;

  } else {

    return gc_sequence_->clear(buffer);

  }
}

void client_session::t1_timeout()
{
  const buffer_type& buf = pkeep_sequence_->send_request();
  notify_send(buf);
}

void client_session::update_event()
{
  notify_event();
}

client_session_service::pointer_type make_client_session_service()
{
  return std::make_shared<client_session>();
}

} // demo
} // asps

