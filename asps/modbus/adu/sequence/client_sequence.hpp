// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU Client sequence.

#ifndef ASPS_MODBUS_ADU_SEQUENCE_CLIENT_SEQUENCE_HPP
#define ASPS_MODBUS_ADU_SEQUENCE_CLIENT_SEQUENCE_HPP

#include <asps/modbus/adu/sequence/sequence.hpp>

namespace asps {
namespace modbus {
namespace adu {

class client_sequence
  : public active_sequence
{
public:
  client_sequence(uint16_t tid, uint8_t uid, const buffer_type& pdu)
  {
    request_ = make_client_request_adu(tid, uid, pdu);
    response_ = make_client_response_adu();
  }
};

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEQUENCE_CLIENT_SEQUENCE_HPP
