// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#include <deque>
#include <sstream>
#include <iterator>
#include <functional>
#include <asps/modbus/api/config.h>
#include <asps/modbus/common/global_event.h>
#include <asps/modbus/session/session.h>
#include <asps/modbus/adu/message/tcp_adu.h>

using namespace asps::modbus;
using namespace std::placeholders;

uint16_t
client_session::mbap_header_size()
{
  return tcp_adu::mbap_header_size();
}

uint16_t
client_session::adu_size(const uint8_t* buffer)
{
  return tcp_adu::mbap_header_size() + tcp_adu::pdu_size(buffer);
}

buffer_list
client_session::read_coils(const coils& cs)
{
  coils::split(cs,
               config::quantity_of_coils(),
               std::back_inserter(coils_queue_));

  return send_request();
}

buffer_list
client_session::send_request()
{
  buffer_list buffers;

  while (sequences_.size() < config::quantity_of_concurrent_requests() &&
         coils_queue_.size() > 0) {

    uint16_t tid = transaction_identifier_++;

    if (!sequences_.count(tid)) {

      coils::pointer_type cs = coils_queue_.front();
      coils_queue_.pop_front();
      sequences_.emplace(tid,
                         std::make_shared<tcp_adu_client_sequence>(
                          tid, unit_identifier_));
      tcp_adu::pointer_type adu =
        sequences_[tid]->get_request(cs, function_codes::read_coils);

      buffers.push_back(std::make_pair(adu->serialize(), adu->serialized_size()));
    }
  }

  return buffers;
}

buffer_list
client_session::receive_response(const uint8_t* buffer)
{
  tcp_adu::pointer_type adu = tcp_adu::unserialize(buffer, false);
  uint16_t tid = adu->transaction_identifier();

  if (sequences_.count(tid)) {
    sequences_[tid]->set_response(adu);
    sequences_.erase(tid);
    return send_request();
  } else {
    client_event* event = global_client_event::instance()->event();
    if (event) {
      std::ostringstream oss;
      oss << "Stray message, transcation identifier: " << tid;
      event->on_error(oss.str());
    }

    return buffer_list();
  }
}

// Chat Server Session
uint16_t
server_session::mbap_header_size()
{
  return tcp_adu::mbap_header_size();
}

uint16_t
server_session::adu_size(const uint8_t* buffer)
{
  return tcp_adu::mbap_header_size() + tcp_adu::pdu_size(buffer);
}

buffer_list
server_session::receive_request(const uint8_t* buffer)
{
  buffer_list buffers;
  tcp_adu::pointer_type request = tcp_adu::unserialize(buffer, true);
  tcp_adu::pointer_type response = sequence_->set_request(request);

  buffers.push_back(std::make_pair(response->serialize(),
                                   response->serialized_size()));

  return buffers;
}
