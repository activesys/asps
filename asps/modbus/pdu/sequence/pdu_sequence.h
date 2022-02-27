// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#ifndef ASPS_MODBUS_PDU_SEQUENCE_PDU_SEQUENCE_H
#define ASPS_MODBUS_PDU_SEQUENCE_PDU_SEQUENCE_H

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
  pdu_client_sequence() {}
  virtual ~pdu_client_sequence() {}

public:
  virtual mb_pdu::pointer_type get_request() = 0;
  virtual void set_response(mb_pdu::pointer_type pdu) = 0;

protected:
  mb_pdu::pointer_type req_;
};

typedef std::shared_ptr<pdu_client_sequence> pdu_client_sequence_ptr;

// Modbus read coils pdu client sequence
class read_coils_pdu_client_sequence : public pdu_client_sequence
{
public:
  read_coils_pdu_client_sequence(const coils::pointer_type cs)
    : coils_(cs)
  {}

public:
  mb_pdu::pointer_type get_request() override;
  void set_response(mb_pdu::pointer_type pdu) override;

private:
  coils::pointer_type coils_;
};

// Modbus write single coil pdu client sequence
class write_single_coil_pdu_client_sequence : public pdu_client_sequence
{
public:
  write_single_coil_pdu_client_sequence(const coils::pointer_type cs)
    : coils_(cs)
  {}

public:
  mb_pdu::pointer_type get_request() override;
  void set_response(mb_pdu::pointer_type pdu) override;

private:
  coils::pointer_type coils_;
};

// Modbus write multiple coils pdu client sequence
class write_multiple_coils_pdu_client_sequence : public pdu_client_sequence
{
public:
  write_multiple_coils_pdu_client_sequence(const coils::pointer_type cs)
    : coils_(cs)
  {}

public:
  mb_pdu::pointer_type get_request() override;
  void set_response(mb_pdu::pointer_type pdu) override;

private:
  coils::pointer_type coils_;
};

// Modbus invalid pdu client sequence
class invalid_pdu_client_sequence : public pdu_client_sequence
{
public:
  invalid_pdu_client_sequence() {}

public:
  mb_pdu::pointer_type get_request() override;
  void set_response(mb_pdu::pointer_type pdu) override;
};

// Modbus PDU Server Sequence
class pdu_server_sequence
{
public:
  typedef std::shared_ptr<pdu_server_sequence> pointer_type;

public:
  virtual mb_pdu::pointer_type set_request(mb_pdu::pointer_type pdu) = 0;
};

// Modbus read coils pdu server sequence
class read_coils_pdu_server_sequence : public pdu_server_sequence
{
public:
  mb_pdu::pointer_type set_request(mb_pdu::pointer_type pdu) override;
};

// Modbus write single coil pdu server sequence
class write_single_coil_pdu_server_sequence : public pdu_server_sequence
{
public:
  mb_pdu::pointer_type set_request(mb_pdu::pointer_type pdu) override;
};

// Modbus write multiple coils pdu server sequence
class write_multiple_coils_pdu_server_sequence : public pdu_server_sequence
{
public:
  mb_pdu::pointer_type set_request(mb_pdu::pointer_type pdu) override;
};

// Modbus invalid pdu server sequence
class invalid_pdu_server_sequence : public pdu_server_sequence
{
public:
  mb_pdu::pointer_type set_request(mb_pdu::pointer_type pdu) override;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_PDU_SEQUENCE_PDU_SEQUENCE_H
