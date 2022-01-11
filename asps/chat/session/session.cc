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
uint16_t client_session::send_msg(const std::string& msg)
{
  uint16_t cid = chat_id_generator::get_chat_id();
  if (!seqs.count(cid)) {
    seqs.emplace(cid, std::make_shared<client_seq>(cid, notify_));
    chat_msg& message = seqs[cid]->get_chat_msg(msg);
    boost::asio::async_write(
      socket_,
      boost::asio::buffer(message.serialize(), message.serialized_size()),
      [this](boost::system::error_code ec, std::size_t /* length */)
      {
        if (ec) {
          notify_.notify_event(ec.message());
        }
      });
  }

  return cid;
}

void client_session::receive_msg()
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
        notify_.notify_event(ec.message());
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
        notify_.notify_event(ec.message());
      } else {
        chat_msg msg = chat_msg::unserialize(buffer_);
        uint16_t cid = msg.id();

        if (seqs.count(cid)) {
          seqs[cid]->set_chat_msg(msg);
          seqs.erase(cid);
        }

        do_read_header();
      }
    });
}
