// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Session.

#include <asps/chat/session/session.h>

using namespace asps::chat;

// Chat ID Generator
uint16_t chat_id_generator::chat_id_ = 0;

// Chat Client Session
uint16_t client_session::send(const std::string& msg)
{
  uint16_t cid = chat_id_generator::get_chat_id();
  if (!seqs_.count(cid)) {
    seqs_.emplace(cid, std::make_shared<client_seq>(cid, event_));
    chat_msg& message = seqs_[cid]->get_chat_msg(msg);
    boost::asio::async_write(
      socket_,
      boost::asio::buffer(message.serialize(), message.serialized_size()),
      [this](boost::system::error_code ec, std::size_t /* length */)
      {
        if (ec && event_) {
          event_->on_error(ec.message());
        }
      });
  }

  return cid;
}

void client_session::start()
{
  do_read_header();
}

void client_session::do_read_header()
{
  buffer_.resize(chat_msg::header_size());
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
        do_read_body(chat_msg::unserialize_header(buffer_));
      }
    });
}

void client_session::do_read_body(uint16_t msg_len)
{
  buffer_.resize(chat_msg::header_size() + msg_len);
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data() + chat_msg::header_size(), msg_len),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (ec) {
        if (event_) {
          event_->on_error(ec.message());
        }
      } else {
        chat_msg msg = chat_msg::unserialize(buffer_);
        uint16_t cid = msg.id();

        if (seqs_.count(cid)) {
          seqs_[cid]->set_chat_msg(msg);
          seqs_.erase(cid);
        } else {
          if (event_) {
            event_->on_error("Stray message: " + msg.msg());
          }
        }

        if (socket_.is_open()) {
          do_read_header();
        }
      }
    });
}

// Chat Server Session
void server_session::start()
{
  session_set_.insert(shared_from_this());
  do_read_header();
}

void server_session::do_read_header()
{
  buffer_.resize(chat_msg::header_size());
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data(), buffer_.size()),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (!ec) {
        do_read_body(chat_msg::unserialize_header(buffer_));
      } else if (ec == boost::asio::error::eof) {
        session_set_.erase(shared_from_this());
      } else if (event_) {
        event_->on_error(ec.message());
      }
    });
}

void server_session::do_read_body(uint16_t msg_len)
{
  buffer_.resize(chat_msg::header_size() + msg_len);
  boost::asio::async_read(
    socket_,
    boost::asio::buffer(buffer_.data() + chat_msg::header_size(), msg_len),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (!ec) {
        chat_msg req_msg = chat_msg::unserialize(buffer_);
        chat_msg rsp_msg = seq_.response(req_msg);
        do_send(rsp_msg);

        do_read_header();
      } else if (ec == boost::asio::error::eof) {
        session_set_.erase(shared_from_this());
      } else if (event_) {
        event_->on_error(ec.message());
      }
    });
}

void server_session::do_send(chat_msg& msg)
{
  boost::asio::async_write(
    socket_,
    boost::asio::buffer(msg.serialize(), msg.serialized_size()),
    [this](boost::system::error_code ec, std::size_t /* length */)
    {
      if (ec == boost::asio::error::eof) {
        session_set_.erase(shared_from_this());
      } else if (ec && event_) {
        event_->on_error(ec.message());
      }
    });
}
