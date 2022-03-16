// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_SESSION_CLIENTSESSION_H
#define ASPS_MODBUS_SESSION_CLIENTSESSION_H

#include <cstdint>
#include <memory>
#include <deque>
#include <list>
#include <unordered_map>
#include <functional>

#include <asps/modbus/api/transport_layer.h>
#include <asps/modbus/api/event.h>
#include <asps/modbus/adu/sequence/tcp_adu_sequence.h>
#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

typedef std::pair<uint8_t*, uint16_t> buffer_type;
typedef std::list<buffer_type> buffer_list;

// Modbus client session
class client_session
{
  typedef std::unordered_map<uint16_t,
                             tcp_adu_client_sequence::pointer_type> sequence_type;
  typedef std::deque<coils::pointer_type> coils_queue_type;

public:
  typedef std::shared_ptr<client_session> pointer_type;

public:
  client_session(uint8_t unit_identifier)
    : transaction_identifier_(0),
      unit_identifier_(unit_identifier)
  {}

public:
  static uint16_t mbap_header_size();
  static uint16_t adu_size(const uint8_t* buffer);

public:
  buffer_list read_coils(const coils& cs);
  buffer_list receive_response(const uint8_t* buffer);

private:
  buffer_list send_request();

private:
  uint16_t transaction_identifier_;
  uint8_t unit_identifier_;
  coils_queue_type coils_queue_;
  sequence_type sequences_;
};

// Modbus Server Session
class server_session
{
public:
  typedef std::shared_ptr<server_session> pointer_type;

public:
  server_session()
    : sequence_(std::make_shared<tcp_adu_server_sequence>())
  {}

public:
  static uint16_t mbap_header_size();
  static uint16_t adu_size(const uint8_t* buffer);

public:
  buffer_list receive_request(const uint8_t* buffer);

private:
  tcp_adu_server_sequence::pointer_type sequence_;
};
/*
class server_session;
typedef std::shared_ptr<server_session> server_session_ptr;
typedef std::unordered_set<server_session_ptr> server_session_set_type;

class server_session : public std::enable_shared_from_this<server_session>
{
public:
  server_session(
    tcp::socket socket,
    server_session_set_type& session_set)
    : socket_(std::move(socket)),
      session_set_(session_set)
  {}

public:
  void start();

private:
  void read_mbap_header();
  void read_pdu(uint16_t length);

private:
  tcp::socket socket_;
  server_session_set_type& session_set_;
  std::vector<uint8_t> buffer_;
};
*/

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SESSION_CLIENTSESSION_H
