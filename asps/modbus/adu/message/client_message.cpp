// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Client Message.

#include <arpa/inet.h>
#include <algorithm>
#include <asps/modbus/adu/message/client_message.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Client ADU
const buffer_type& client_request_adu::serialize()
{
  buffer_.resize(mbap_field_length + pdu_.size());
  uint8_t* pos = buffer_.data();

  // encode transaction identifier
  *reinterpret_cast<uint16_t*>(pos) = htons(tid_);
  pos += transaction_identifier_field_length;

  // encode protocol identifier
  *reinterpret_cast<uint16_t*>(pos) = htons(pid_);
  pos += protocol_identifier_field_length;

  // encode length
  *reinterpret_cast<uint16_t*>(pos) = htons(pdu_.size() + 1);
  pos += length_field_length;

  // encode unit identifier
  *pos = uid_;
  pos += unit_identifier_field_length;

  // encode pdu
  std::copy(pdu_.begin(), pdu_.end(), buffer_.begin() + mbap_field_length);

  return buffer_;
}

bool client_response_adu::unserialize(const buffer_type& buffer)
{
  const uint8_t* pos = buffer.data();

  // decode transaction identifier
  tid_ = ntohs(*reinterpret_cast<const uint16_t*>(pos));
  pos += transaction_identifier_field_length;

  // decode protocol identifier
  pid_ = ntohs(*reinterpret_cast<const uint16_t*>(pos));
  pos += protocol_identifier_field_length;

  // decode length
  uint16_t pdu_length = ntohs(*reinterpret_cast<const uint16_t*>(pos));
  pos += length_field_length;

  // decode unit identifier
  uid_ = *pos;
  pos += unit_identifier_field_length;

  // decode pdu
  // The adu length contains the uid length.
  pdu_length -= unit_identifier_field_length;
  pdu_.resize(pdu_length);
  std::copy(buffer.begin() + mbap_field_length,
            buffer.begin() + mbap_field_length + pdu_length,
            pdu_.begin());

  return true;
}

} // adu
} // modbus
} // asps
