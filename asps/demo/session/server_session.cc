// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Session.

#include <functional>
#include <asps/demo/config/config.h>
#include <asps/demo/session/server_session.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

// Server Session
server_session_service::pointer_type
make_server_session_service()
{
  return std::make_shared<server_session>();
}

server_session::server_session()
  : data_sequence_(make_data_receive_sequence()),
    nkeep_sequence_(make_active_sequence(true)),
    gc_sequence_(make_garbage_collector_sequence()),
    t1_(make_timer_service(config::t1() * 1000,
                           std::bind(&server_session::t1_timeout, this),
                           true))
{
  data_sequence_->register_data_observer(this);
  nkeep_sequence_->register_event_observer(this);
  t1_->start();
}

server_session::~server_session()
{
  t1_->stop();
}

void server_session::receive(buffer_type& buffer)
{
  while (receive_one_package(buffer));
}

bool server_session::receive_one_package(buffer_type& buffer)
{
  sequence_type type = get_server_sequence_type(buffer);

  if (type == belong_to_data_sequence) {

    return data_sequence_->receive_data(buffer);

  } else if (type == belong_to_positive_keepalive_sequence) {

    passive_sequence_service::pointer_type seq = make_passive_sequence(true);
    if (!seq->receive_request(buffer)) {
      return false;
    }

    const buffer_type& buf = seq->send_response();
    notify_send(buf);

    return true;

  } else if (type == belong_to_negative_keepalive_sequence) {

    return nkeep_sequence_->receive_response(buffer);

  } else {

    return gc_sequence_->clear(buffer);

  }
}

void server_session::t1_timeout()
{
  const buffer_type& buf = nkeep_sequence_->send_request();
  notify_send(buf);
}

void server_session::update_data(const data_group_type& datas)
{
  notify_data(datas);
}

void server_session::update_event()
{
  notify_event();
}

} // demo
} // asps
