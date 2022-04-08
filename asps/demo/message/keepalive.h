// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_KEEPALIVE_H
#define ASPS_DEMO_MESSAGE_KEEPALIVE_H

#include <cstdint>
#include <cstring>
#include <vector>

#include <asps/demo/session/message_service.h>

namespace asps {
namespace demo {

class positive_keepalive : public message_serialization_service
{
  enum flag_length {
    flag_field_length = 4
  };

  typedef std::vector<uint8_t> buffer_type;

public:
  const buffer_type& serialize() override
  {
    buffer_.resize(flag_field_length);
    uint8_t* pos = buffer_.data();

    // flag field is "KEEP"
    memcpy(reinterpret_cast<char*>(pos), "KEEP", flag_field_length);
    pos += flag_field_length;

    return buffer_;
  }

private:
  buffer_type buffer_;
};

class positive_keepalive_ack : public message_unserialization_service
{
  enum flag_length {
    flag_field_length = 1
  };

public:
  explicit positive_keepalive_ack(uint8_t flag)
    : message_unserialization_service(),
      flag_(flag)
  {}

public:
  bool unserialize(const uint8_t* buffer) override
  {
    return buffer != nullptr && *buffer == flag_;
  }

private:
  uint8_t flag_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_KEEPALIVE_H
