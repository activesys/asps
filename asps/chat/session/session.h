// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Session.

#ifndef ASPS_CHAT_SESSION_SESSION_H
#define ASPS_CHAT_SESSION_SESSION_H

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <boost/asio.hpp>

#include <asps/chat/notify.h>
#include <asps/chat/msg/chat_seq.h>

namespace asps {
namespace chat {

using boost::asio::ip::tcp;

// Chat ID Generator
class chat_id_generator
{
public:
  static uint16_t get_chat_id() { return chat_id_++; }

private:
  static uint16_t chat_id_;
};

// Chat Client Session
class client_session
{
public:
  client_session(tcp::socket& sock, client_notify& notify)
    : socket_(sock),
      notify_(notify)
  {}

public:
  uint16_t send_msg(const std::string& msg);
  void receive_msg();

private:
  void do_read_header();
  void do_read_body(uint16_t msg_len);

private:
  std::unordered_map<uint16_t, client_seq_ptr> seqs;
  tcp::socket& socket_;
  std::vector<uint8_t> buffer_;
  client_notify& notify_;
};

typedef std::shared_ptr<client_session> client_session_ptr;

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_SESSION_SESSION_H
