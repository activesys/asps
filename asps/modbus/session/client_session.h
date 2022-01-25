// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_SESSION_CLIENTSESSION_H
#define ASPS_MODBUS_SESSION_CLIENTSESSION_H

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

#include <boost/asio.hpp>

#include <asps/modbus/api/event.h>
#include <asps/modbus/adu/sequence/tcp_adu_sequence.h>
#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

using boost::asio::ip::tcp;

// Modbus client session
class client_session
{
  typedef std::unordered_map<uint16_t, tcp_adu_client_sequence_ptr> sequence_type;
  typedef std::unordered_map<uint16_t, std::deque<coils::ptr_type>> coils_queue_type;

public:
  client_session(tcp::socket& socket, client_event* event)
    : transaction_identifier_(0),
      socket_(socket),
      event_(event)
  {}

public:
  void start();
  void read_coils(uint8_t unit_identifier, const coils& cs);

private:
  void read_mbap_header();
  void read_pdu(uint16_t length);
  void send_read_coils_request();

private:
  uint16_t transaction_identifier_;
  tcp::socket& socket_;
  client_event* event_;
  std::vector<uint8_t> buffer_;
  sequence_type sequences_;
  coils_queue_type coils_queue_;
};

typedef std::shared_ptr<client_session> client_session_ptr;

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SESSION_CLIENTSESSION_H
