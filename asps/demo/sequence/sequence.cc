// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Sequence.

#include <asps/demo/config/config.h>
#include <asps/demo/sequence/sequence.h>
#include <asps/demo/sequence/client_sequence.h>
#include <asps/demo/sequence/server_sequence.h>

namespace asps {
namespace demo {

sequence_type get_client_sequence_type(const buffer_type& buffer)
{
  message_type type = get_message_type(buffer);

  // For the client, only Pack and nkeep are valid data
  if (type == positive_keepalive_ack_message) {
    return belong_to_positive_keepalive_sequence;
  } else if (type == negative_keepalive_message) {
    return belong_to_negative_keepalive_sequence;
  } else {
    return belong_to_invalid_sequence;
  }
}

sequence_type get_server_sequence_type(const buffer_type& buffer)
{
  message_type type = get_message_type(buffer);

  // For the server, only data, pkeep and nack are valid data
  if (type == data_message) {
    return belong_to_data_sequence;
  } else if (type == positive_keepalive_message) {
    return belong_to_positive_keepalive_sequence;
  } else if (type == negative_keepalive_ack_message) {
    return belong_to_negative_keepalive_sequence;
  } else {
    return belong_to_invalid_sequence;
  }
}

// Sequence make function
data_send_sequence_service::pointer_type
make_data_send_sequence()
{
  return std::make_shared<client_data_sequence>();
}

data_receive_sequence_service::pointer_type
make_data_receive_sequence()
{
  return std::make_shared<server_data_sequence>();
}

active_sequence_service::pointer_type
make_active_sequence(bool is_server)
{
  if (is_server) {
    return std::make_shared<server_negative_keepalive_sequence>();
  } else {
    return std::make_shared<client_positive_keepalive_sequence>();
  }
}

passive_sequence_service::pointer_type
make_passive_sequence(bool is_server)
{
  if (is_server) {
    return std::make_shared<server_positive_keepalive_sequence>();
  } else {
    return std::make_shared<client_negative_keepalive_sequence>();
  }
}

garbage_collector_sequence_service::pointer_type
make_garbage_collector_sequence()
{
  return std::make_shared<garbage_collector_sequence>();
}

// Passive Sequence
passive_sequence::passive_sequence()
{}

const buffer_type& passive_sequence::send_response()
{
  return ack_->serialize();
}

bool passive_sequence::receive_request(buffer_type& buffer)
{
  return keepalive_->unserialize(buffer);
}

// Active Sequence
active_sequence::active_sequence()
  : t2_(make_timer_service(config::t2(),
                           std::bind(&active_sequence::t2_timeout, this))),
    state_(none_state::instance())
{}

void active_sequence::t2_start()
{
  t2_->start();
}

void active_sequence::t2_stop()
{
  t2_->stop();
}

const buffer_type& active_sequence::serialize()
{
  return keepalive_->serialize();
}

bool active_sequence::unserialize(buffer_type& buffer)
{
  return ack_->unserialize(buffer);
}

const buffer_type& active_sequence::send_request()
{
  return state_->request(this);
}

void active_sequence::change_state(state* s)
{
  state_ = s;
}

bool active_sequence::receive_response(buffer_type& buffer)
{
  return state_->response(this, buffer);
}

void active_sequence::t2_timeout()
{
  state_->timeout(this);
}

// Garbage Collector Sequence
garbage_collector_sequence::garbage_collector_sequence()
  : message_(make_invalid_message())
{}

bool garbage_collector_sequence::clear(buffer_type& buffer)
{
  return message_->unserialize(buffer);
}

} // demo
} // asps
