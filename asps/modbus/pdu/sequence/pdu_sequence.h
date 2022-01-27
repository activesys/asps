// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#ifndef ASPS_MODBUS_PDU_PDUSEQUENCE_H
#define ASPS_MODBUS_PDU_PDUSEQUENCE_H

#include <memory>
#include <asps/modbus/pdu/message/pdu.h>
#include <asps/modbus/semantic/coils.h>
#include <asps/modbus/api/event.h>

namespace asps {
namespace modbus {

// Modbus PDU Client Sequence
class pdu_client_sequence
{
public:
  pdu_client_sequence(client_event* event)
    : event_(event)
  {}
  virtual ~pdu_client_sequence() {}

public:
  virtual pdu_ptr get_request(bool is_read) = 0;
  virtual void set_response(pdu_ptr pdu) = 0;

protected:
  client_event* event_;
};

typedef std::shared_ptr<pdu_client_sequence> pdu_client_sequence_ptr;

// Modbus read coils pdu client sequence
class read_coils_pdu_client_sequence : public pdu_client_sequence
{
public:
  read_coils_pdu_client_sequence(
    const coils::ptr_type cs,
    client_event* event)
    : pdu_client_sequence(event),
      coils_(cs)
  {}

public:
  pdu_ptr get_request(bool is_read) override;
  void set_response(pdu_ptr pdu) override;

public:
  coils::ptr_type coils_;
};

// Modbus PDU Server Sequence
class pdu_server_sequence
{
public:
  pdu_server_sequence(server_event* event)
    : event_(event)
  {}
  virtual ~pdu_server_sequence() {}

public:
  virtual pdu_ptr set_request(pdu_ptr pdu) = 0;

protected:
  server_event* event_;
};

typedef std::shared_ptr<pdu_server_sequence> pdu_server_sequence_ptr;

// Modbus read coils pdu server sequence
class read_coils_pdu_server_sequence : public pdu_server_sequence
{
public:
  read_coils_pdu_server_sequence(server_event* event)
    : pdu_server_sequence(event)
  {}

public:
  pdu_ptr set_request(pdu_ptr pdu) override;
};

// Modbus invalid pdu server sequence
class invalid_pdu_server_sequence : public pdu_server_sequence
{
public:
  invalid_pdu_server_sequence(server_event* event)
    : pdu_server_sequence(event)
  {}

public:
  pdu_ptr set_request(pdu_ptr pdu) override;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_PDU_PDUSEQUENCE_H
