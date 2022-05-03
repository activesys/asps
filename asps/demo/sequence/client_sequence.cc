// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Sequence.

#include <asps/demo/config/config.h>
#include <asps/demo/sequence/client_sequence.h>

namespace asps {
namespace demo {

sequence_type get_sequence_type(const buffer_type& buffer)
{
  message_type type = get_message_type(buffer);

  /*
   * For the client, only Pack and nkeep are valid data
   */
  if (type == positive_keepalive_ack_message) {
    return belong_to_positive_keepalive_sequence;
  } else if (type == negative_keepalive_message) {
    return belong_to_negative_keepalive_sequence;
  } else {
    return belong_to_invalid_sequence;
  }
}

// Client Data Sequence
client_data_sequence_service::pointer_type
make_client_data_sequence()
{
  return std::make_shared<client_data_sequence>();
}

const buffer_type& client_data_sequence::request(const data_group_type& group)
{
  message_ = make_message_serialization_service(group);
  return message_->serialize();
}

// Client Positive Keepalive Sequence
client_positive_keepalive_sequence_service::pointer_type
make_client_positive_keepalive_sequence()
{
  return std::make_shared<client_positive_keepalive_sequence>();
}

client_positive_keepalive_sequence::client_positive_keepalive_sequence()
  : keepalive_(make_message_serialization_service(true)),
    ack_(make_message_unserialization_service(config::pack())),
    t2_(make_timer_service(config::t2(),
                           std::bind(&client_positive_keepalive_sequence::t2_timeout,
                                     this))),
    state_(none_state::instance())
{
}

void client_positive_keepalive_sequence::t2_start()
{
  t2_->start();
}

void client_positive_keepalive_sequence::t2_stop()
{
  t2_->stop();
}

const buffer_type& client_positive_keepalive_sequence::serialize()
{
  return keepalive_->serialize();
}

bool client_positive_keepalive_sequence::unserialize(buffer_type& buffer)
{
  return ack_->unserialize(buffer);
}

const buffer_type& client_positive_keepalive_sequence::request()
{
  return state_->request(this);
}

void client_positive_keepalive_sequence::change_state(state* s)
{
  state_ = s;
}

bool client_positive_keepalive_sequence::response(buffer_type& buffer)
{
  return state_->response(this, buffer);
}

void client_positive_keepalive_sequence::t2_timeout()
{
  state_->timeout(this);
}

// Client Negative Keepalive Sequence
client_negative_keepalive_sequence_service::pointer_type
make_client_negative_keepalive_sequence()
{
  return std::make_shared<client_negative_keepalive_sequence>();
}

client_negative_keepalive_sequence::client_negative_keepalive_sequence()
{
  keepalive_ = make_message_unserialization_service(config::nkeep());
  ack_ = make_message_serialization_service(false);
}

const buffer_type& client_negative_keepalive_sequence::request()
{
  return ack_->serialize();
}

bool client_negative_keepalive_sequence::response(buffer_type& buffer)
{
  return keepalive_->unserialize(buffer);
}

// Invalid Sequence
client_invalid_sequence_service::pointer_type
make_client_invalid_sequence()
{
  return std::make_shared<client_invalid_sequence>();
}

client_invalid_sequence::client_invalid_sequence()
{
  message_ = make_invalid_message();
}

bool client_invalid_sequence::response(buffer_type& buffer)
{
  return message_->unserialize(buffer);
}

} // demo
} // asps
