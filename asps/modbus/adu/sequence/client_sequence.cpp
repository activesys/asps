// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU Client sequence.

#include <asps/modbus/adu/sequence/client_sequence.hpp>

namespace asps {
namespace modbus {
namespace adu {

active_sequence_service::pointer_type
make_active_sequence(uint16_t tid, uint8_t uid, const buffer_type& pdu)
{
  return std::make_shared<client_sequence>(tid, uid, pdu);
}

} // adu
} // modbus
} // asps
