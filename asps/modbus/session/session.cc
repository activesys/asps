// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#include <deque>
#include <sstream>
#include <iterator>
#include <asps/modbus/api/config.h>
#include <asps/modbus/common/global_event.h>
#include <asps/modbus/session/session.h>
#include <asps/modbus/adu/message/tcp_adu.h>

using namespace asps::modbus;

uint16_t client_session::mbap_header_size()
{
  return tcp_adu::mbap_header_size();
}

uint16_t client_session::adu_size(const uint8_t* buffer)
{
  return tcp_adu::mbap_header_size() + tcp_adu::pdu_size(buffer);
}

void client_session::read_coils(const coils& cs)
{
  coils::split(cs, config::quantity_of_coils(), std::back_inserter(coils_queue_));
  send_request();
}

void client_session::send_request()
{
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

      transport_layer_.write(adu->serialize(), adu->serialized_size());
    }
  }
}

void client_session::receive_response(const uint8_t* buffer)
{
  tcp_adu::pointer_type adu = tcp_adu::unserialize(buffer, false);
  uint16_t tid = adu->transaction_identifier();

  if (sequences_.count(tid)) {
    sequences_[tid]->set_response(adu);
    sequences_.erase(tid);
    send_request();
  } else {
    client_event* event = global_client_event::instance()->event();
    if (event) {
      std::ostringstream oss;
      oss << "Stray message, transcation identifier: " << tid;
      event->on_error(oss.str());
    }
  }
}

// Chat Server Session
void server_session::start()
{
  session_set_.insert(shared_from_this());
  read_mbap_header();
}

void server_session::read_mbap_header()
{
  buffer_.resize(tcp_adu::mbap_header_size());
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data(), buffer_.size()),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (!ec) {
        read_pdu(tcp_adu::pdu_size(buffer_.data()));
      } else if (ec == boost::asio::error::eof) {
        session_set_.erase(shared_from_this());
      } else if (event_) {
        event_->on_error(ec.message());
      }
    });
}

void server_session::read_pdu(uint16_t length)
{
  buffer_.resize(tcp_adu::mbap_header_size() + length);
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data() + tcp_adu::mbap_header_size(), length),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (!ec) {
        tcp_adu::pointer_type request = tcp_adu::unserialize(buffer_.data(), true);
        tcp_adu_server_sequence sequence(event_);
        tcp_adu::pointer_type response = sequence.set_request(request);
        boost::asio::async_write(
          socket_,
          boost::asio::buffer(response->serialize(), response->serialized_size()),
          [this](boost::system::error_code ec, std::size_t /* length */)
          {
            if (ec == boost::asio::error::eof) {
              session_set_.erase(shared_from_this());
            } else if (ec && event_) {
              event_->on_error(ec.message());
            }
          });

        if (socket_.is_open()) {
          read_mbap_header();
        }
      } else if (ec == boost::asio::error::eof) {
        session_set_.erase(shared_from_this());
      } else if (event_) {
        event_->on_error(ec.message());
      }
    });
}
