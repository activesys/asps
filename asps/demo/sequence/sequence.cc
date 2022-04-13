// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Sequence.

#include <asps/demo/config/config.h>
#include <asps/demo/sequence/sequence.h>

namespace asps {
namespace demo {

sequence_type get_sequence_type(const uint8_t* buffer)
{
  message_type type = get_message_type(buffer);

  if (type == data_message) {
    return belong_to_data_sequence;
  }

  if (type == positive_keepalive_message ||
      type == positive_keepalive_ack_message) {
    return belong_to_positive_keepalive_sequence;
  }

  if (type == negative_keepalive_message ||
      type == negative_keepalive_ack_message) {
    return belong_to_negative_keepalive_sequence;
  }

  return belong_to_invalid_sequence;
}

sequence_service::pointer_type
make_sequence_service(const data_group_type& group)
{
  return std::make_shared<data_sequence>(group);
}

sequence_service::pointer_type
make_sequence_service(bool positive)
{
  if (positive) {
    return std::make_shared<positive_keepalive_sequence>();
  } else {
    return std::make_shared<negative_keepalive_sequence>();
  }
}

data_sequence::data_sequence(const data_group_type& group)
{
  message_ = make_message_serialization_service(group);
}

const buffer_type& data_sequence::request()
{
  return message_->serialize();
}

bool data_sequence::response(const uint8_t* buffer)
{
  return false;
}

positive_keepalive_sequence::positive_keepalive_sequence()
  : keepalive_(make_message_serialization_service(true)),
    ack_(make_message_unserialization_service(config::pack())),
    t2_(make_timer_service()),
    state_(none_state::instance())
{
}

void positive_keepalive_sequence::t2_start()
{
  t2_->start(std::chrono::seconds(config::t2()),
             std::bind(&positive_keepalive_sequence::t2_timeout, this));
}

void positive_keepalive_sequence::t2_stop()
{
  t2_->stop();
}

const buffer_type& positive_keepalive_sequence::serialize()
{
  return keepalive_->serialize();
}

bool positive_keepalive_sequence::unserialize(const uint8_t* buffer)
{
  return ack_->unserialize(buffer);
}

const buffer_type& positive_keepalive_sequence::request()
{
  return state_->request(this);
}

void positive_keepalive_sequence::change_state(state* s)
{
  state_ = s;
}

bool positive_keepalive_sequence::response(const uint8_t* buffer)
{
  return state_->response(this, buffer);
}

void positive_keepalive_sequence::t2_timeout()
{
  state_->timeout(this);
}

negative_keepalive_sequence::negative_keepalive_sequence()
{
  keepalive_ = make_message_unserialization_service(config::nkeep());
  ack_ = make_message_serialization_service(false);
}

const buffer_type& negative_keepalive_sequence::request()
{
  return ack_->serialize();
}

bool negative_keepalive_sequence::response(const uint8_t* buffer)
{
  return keepalive_->unserialize(buffer);
}

} // demo
} // asps
