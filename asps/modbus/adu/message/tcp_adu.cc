// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU.

#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <asps/modbus/adu/message/tcp_adu.h>

using namespace asps::modbus;

// Modbus TCP ADU
std::size_t tcp_adu::mbap_header_size()
{
  return transaction_identifier_field_length +
         protocol_identifier_field_length +
         length_field_length +
         unit_identifier_field_length;
}

uint16_t tcp_adu::pdu_size(std::vector<uint8_t>& buffer)
{
  uint8_t* pos = buffer.data();
  pos += transaction_identifier_field_length + protocol_identifier_field_length;

  return ntohs(*reinterpret_cast<uint16_t*>(pos)) - unit_identifier_field_length;
}

tcp_adu tcp_adu::unserialize(std::vector<uint8_t>& buffer, bool is_request)
{
  uint8_t* pos = buffer.data();
  // decode transaction identifier
  uint16_t transaction_identifier = ntohs(*reinterpret_cast<uint16_t*>(pos));
  pos += transaction_identifier_field_length;
  // decode protocol identifier
  uint16_t protocol_identifier = ntohs(*reinterpret_cast<uint16_t*>(pos));
  pos += protocol_identifier_field_length;
  // decode length
  uint16_t length = ntohs(*reinterpret_cast<uint16_t*>(pos));
  pos += length_field_length;
  // decode unit identifier
  uint8_t unit_identifier = *pos;
  pos += unit_identifier_field_length;
  // decode pdu
  pdu_ptr pdu = pdu::unserialize(pos, is_request);
  pos += length - 1;

  return tcp_adu(transaction_identifier, unit_identifier, pdu);
}

std::size_t tcp_adu::serialized_size()
{
  return transaction_identifier_field_length +
         protocol_identifier_field_length +
         length_field_length +
         unit_identifier_field_length +
         pdu_->serialized_size();
}

uint8_t* tcp_adu::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode transaction identifier
  *reinterpret_cast<uint16_t*>(pos) = htons(transaction_identifier_);
  pos += transaction_identifier_field_length;
  // encode protocol identifier
  *reinterpret_cast<uint16_t*>(pos) = htons(default_protocol_identifier);
  pos += protocol_identifier_field_length;
  // encode length
  *reinterpret_cast<uint16_t*>(pos) = htons(1 + pdu_->serialized_size());
  pos += length_field_length;
  // encode unit identifier
  *pos = unit_identifier_;
  pos += unit_identifier_field_length;
  // encode pdu
  uint8_t* pdu_pos = pdu_->serialize();
  memcpy(pos, pdu_pos, pdu_->serialized_size());
  pos += pdu_->serialized_size();

  return buffer_.data();
}
