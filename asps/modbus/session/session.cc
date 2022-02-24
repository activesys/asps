// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#include <deque>
#include <sstream>
#include <asps/modbus/api/config.h>
#include <asps/modbus/session/session.h>
#include <asps/modbus/adu/message/tcp_adu.h>

using namespace asps::modbus;

void client_session::read_coils(const coils& cs)
{
  coils::split(cs, config::quantity_of_coils(), coils_queue_.end());
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
      sequences_.emplace(
        tid,
        std::make_shared<tcp_adu_client_sequence>(
          tid, unit_identifier_, event_));

      tcp_adu::pointer_type adu =
        sequences_[tid]->get_request(cs, function_codes::read_coils);
        /*
      boost::asio::async_write(
        socket_,
        boost::asio::buffer(adu->serialize(), adu->serialized_size()),
        [this](boost::system::error_code ec, std::size_t length)
        {
          if (ec && event_) {
            event_->on_error(ec.message());
          }
        });
        */
    }
  }
}

#if 0
// Modbus client session
void client_session::start()
{
  read_mbap_header();
}

void client_session::read_mbap_header()
{
  buffer_.resize(tcp_adu::mbap_header_size());
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data(), buffer_.size()),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (ec) {
        if (event_) {
          event_->on_error(ec.message());
        }
      } else {
        read_pdu(tcp_adu::pdu_size(buffer_.data()));
      }
    });
}

void client_session::read_pdu(uint16_t length)
{
  buffer_.resize(tcp_adu::mbap_header_size() + length);
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data() + tcp_adu::mbap_header_size(), length),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (ec) {
        if (event_) {
          event_->on_error(ec.message());
        }
      } else {
        tcp_adu::pointer_type adu = tcp_adu::unserialize(buffer_.data(), false);
        uint16_t tid = adu->transaction_identifier();

        if (sequences_.count(tid)) {
          sequences_[tid]->set_response(adu);
          sequences_.erase(tid);
          send_read_coils_request();
        } else {
          if (event_) {
            std::ostringstream oss;
            oss << "Stray message, transcation identifier: " << tid;
            event_->on_error(oss.str());
          }
        }

        if (socket_.is_open()) {
          read_mbap_header();
        }
      }
    });
}

void client_session::read_coils(uint8_t unit_identifier, const coils& cs)
{
  //coils::split(cs, config::quantity_of_coils(), coils_queue_[unit_identifier]);
  send_read_coils_request();
}

void client_session::send_read_coils_request()
{
  for (coils_queue_type::iterator it = coils_queue_.begin();
       it != coils_queue_.end();
       ++it) {

    while (sequences_.size() < config::quantity_of_concurrent_requests() &&
           it->second.size() > 0) {

      uint16_t tid = transaction_identifier_++;
      if (!sequences_.count(tid)) {

        coils::pointer_type cs = it->second.front();
        it->second.pop_front();
        sequences_.emplace(
          tid,
          std::make_shared<tcp_adu_client_sequence>(
            tid, it->first, event_));

        tcp_adu::pointer_type adu =
          sequences_[tid]->get_request(cs, function_codes::read_coils);
        boost::asio::async_write(
          socket_,
          boost::asio::buffer(adu->serialize(), adu->serialized_size()),
          [this](boost::system::error_code ec, std::size_t /* length */)
          {
            if (ec && event_) {
              event_->on_error(ec.message());
            }
          });
      }
    }
  }
}
#endif

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
