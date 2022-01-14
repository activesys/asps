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
#include <set>
#include <unordered_map>
#include <vector>
#include <boost/asio.hpp>

#include <asps/chat/api/event.h>
#include <asps/chat/sequence/chat_seq.h>

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
class client_session;
typedef std::shared_ptr<client_session> client_session_ptr;

class client_session
{
public:
  client_session(tcp::socket& sock, client_event* event)
    : socket_(sock),
      event_(event)
  {}

public:
  uint16_t send(const std::string& msg);
  void start();

private:
  void do_read_header();
  void do_read_body(uint16_t msg_len);

private:
  std::unordered_map<uint16_t, client_seq_ptr> seqs_;
  tcp::socket& socket_;
  std::vector<uint8_t> buffer_;
  client_event* event_;
};

// Chat Server Session
class server_session;
typedef std::shared_ptr<server_session> server_session_ptr;
typedef std::set<server_session_ptr> server_session_set_type;

class server_session : public std::enable_shared_from_this<server_session>
{
public:
  server_session(
    tcp::socket socket,
    server_session_set_type& session_set,
    server_event* event)
    : socket_(std::move(socket)),
      session_set_(session_set),
      event_(event),
      seq_(event)
  {}

public:
  void start();

private:
  void do_read_header();
  void do_read_body(uint16_t msg_len);
  void do_send(chat_msg& msg);

private:
  tcp::socket socket_;
  server_session_set_type& session_set_;
  std::vector<uint8_t> buffer_;
  server_seq seq_;
  server_event* event_;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_SESSION_SESSION_H
