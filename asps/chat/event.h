// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Event.

#ifndef ASPS_CHAT_EVENT_H
#define ASPS_CHAT_EVENT_H

#include <cstdint>
#include <string>

namespace asps {
namespace chat {

// Client Event
class client;
class client_event
{
public:
  explicit client_event(client& c)
    : chat_client(c)
  {}
  virtual ~client_event() {}

public:
  virtual void on_connect(const std::string& address, uint16_t port) = 0;
  virtual void on_connect(const std::string& err_msg) = 0;

  virtual void on_receive(uint16_t chat_id, const std::string& message) = 0;
  virtual void on_error(const std::string& err_msg) = 0;

protected:
  client& chat_client;
};

// Server event
class server;
class server_event
{
public:
  explicit server_event(server& s)
    : chat_server(s)
  {}
  virtual ~server_event() {}

public:
  virtual void on_accept(const std::string& address, uint16_t port) = 0;
  virtual void on_accept(const std::string& err_msg) = 0;

  virtual std::string on_receive(uint16_t chat_id, const std::string& message) = 0;
  virtual void on_error(const std::string& err_msg) = 0;

protected:
  server& chat_server;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_EVENT_H
