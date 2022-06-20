// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus Client PDU Sequence.

#ifndef ASPS_MODBUS_PDU_SEQUENCE_CLIENT_SEQUENCE_HPP
#define ASPS_MODBUS_PDU_SEQUENCE_CLIENT_SEQUENCE_HPP

#include <asps/modbus/pdu/semantic/request.hpp>
#include <asps/modbus/pdu/sequence/sequence_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

class client_read_coils_sequence
  : public active_sequence_service
{
public:
  client_read_coils_sequence(const request::pointer_type& req)
    : active_sequence_service(req)
  {
    const read_coils_request* r =
      dynamic_cast<const read_coils_request*>(req.get());
    if (r) {
      request_ = make_client_read_coils_request(r->address(), r->quantity());
      response_ = make_client_read_coils_response();
    }
  }

private:
  virtual void sync_response() override;
};

} // namespace pdu
} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_PDU_SEQUENCE_CLIENT_SEQUENCE_HPP
