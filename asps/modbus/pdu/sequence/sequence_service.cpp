// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Active Sequence Interface.

#include <asps/modbus/pdu/sequence/sequence_service.hpp>
#include <asps/modbus/pdu/sequence/client_sequence.hpp>

namespace asps {
namespace modbus {
namespace pdu {

/*
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
*/

// Sequence make function
active_sequence_service::pointer_type
make_active_sequence(request::pointer_type req)
{
  switch (req->func_code()) {
  case function_code_read_coils:
    return std::make_shared<client_read_coils_sequence>(req);
    break;
  
  case function_code_read_discrete_inputs:
    return std::make_shared<client_read_discrete_inputs_sequence>(req);
    break;
  }

  return nullptr;
}

/*
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
*/

/*
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
*/

// Active Sequence
active_sequence_service::active_sequence_service(const request::pointer_type& req)
  : req_(req),
    state_(idle_state::instance())
{}

const buffer_type& active_sequence_service::serialize()
{
  return request_->serialize();
}

bool active_sequence_service::unserialize(const buffer_type& buffer)
{
  return response_->unserialize(buffer);
}

void active_sequence_service::notify()
{
  sync_response();

  if (response_->excep_code()) {
    notify_exception(response_->excep_code());
  } else {
    notify_datas(response_->datas());
  }
}

const buffer_type& active_sequence_service::send_request()
{
  return state_->request(this);
}

void active_sequence_service::change_state(state* s)
{
  state_ = s;
}

bool active_sequence_service::receive_response(const buffer_type& buffer)
{
  return state_->response(this, buffer);
}

/*
// Garbage Collector Sequence
garbage_collector_sequence::garbage_collector_sequence()
  : message_(make_invalid_message())
{}

bool garbage_collector_sequence::clear(buffer_type& buffer)
{
  return message_->unserialize(buffer);
}
*/

} // pdu
} // modbus
} // asps
