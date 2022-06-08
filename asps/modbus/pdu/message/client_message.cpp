// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Message service.

#include <arpa/inet.h>
#include <asps/modbus/pdu/message/client_message.hpp>
#include <asps/modbus/pdu/semantic/constant.hpp>

namespace asps {
namespace modbus {
namespace pdu {

message_serialization_service::pointer_type
make_client_read_coils_request(uint16_t starting_address,
                               uint16_t quantity_of_coils)
{
  return std::make_shared<client_read_coils_request>(starting_address,
                                                     quantity_of_coils);
}

message_unserialization_service::pointer_type
make_client_read_coils_response()
{
  return std::make_shared<client_exception>(
          std::make_shared<client_read_coils_response>());
}

bool client_exception::unserialize(const buffer_type& buffer)
{
  const uint8_t* pos = buffer.data();

  if (*pos & 0x80) {
    // decode function code
    fc_ = function_code(*pos & 0x7f);
    pos += 1;

    // decode exception code
    ec_ = exception_code(*pos);
    pos += 1;

    return true;
  } else {
    bool ret = response_->unserialize(buffer);
    fc_ = response_->func_code();
    ec_ = response_->excep_code();

    return ret;
  }
}

const buffer_type& client_read_coils_request::serialize()
{
  buffer_.resize(5);
  uint8_t* pos = buffer_.data();

  // encode function code;
  *pos = function_code_read_coils;
  pos += 1;
  // encode starting address
  *reinterpret_cast<uint16_t*>(pos) = htons(starting_address_);
  pos += 2;
  // encode quantity of coils
  *reinterpret_cast<uint16_t*>(pos) = htons(quantity_of_coils_);
  pos += 2;

  return buffer_;
}

bool client_read_coils_response::unserialize(const buffer_type& buffer)
{
  const uint8_t* pos = buffer.data();

  // deocde function code
  fc_ = function_code(*pos);
  pos += 1;

  // decode byte count
  uint8_t byte_count = *pos;
  pos += 1;

  // decode status
  status_.resize(byte_count * 8);
  for (std::size_t i = 0; i < byte_count * 8; ++i) {
    status_[i].bit = (pos[i/8] >> i%8) & 0x01;
  }
  pos += byte_count;

  return true;
}

} // pdu
} // modbus
} // asps
