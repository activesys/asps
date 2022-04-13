// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session Message service.

#ifndef ASPS_DEMO_SEQUENCE_MESSAGE_SERVICE_H
#define ASPS_DEMO_SEQUENCE_MESSAGE_SERVICE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

enum message_type
{
  data_message,
  positive_keepalive_message,
  positive_keepalive_ack_message,
  negative_keepalive_message,
  negative_keepalive_ack_message,
  invalid_message
};

static const uint8_t DATA_MESSAGE_FLAG[] = {
  // header flag 'DEMOV100'
  0x44, 0x45, 0x4d, 0x4f, 0x56, 0x31, 0x30, 0x30
};
static const uint8_t POSITIVE_KEEPALIVE_MESSAGE_FLAG[] = {
  // positive keepalive flag 'KEEP'
  0x4b, 0x45, 0x45, 0x50
};
static const uint8_t NEGATIVE_KEEPALIVE_ACK_MESSAGE_FLAG[] = {
  // positive keepalive flag 'KACK'
  0x4b, 0x41, 0x43, 0x4b
};

message_type get_message_type(const uint8_t* buffer);

class message_serialization_service
{
public:
  typedef std::shared_ptr<message_serialization_service> pointer_type;

public:
  message_serialization_service() = default;
  virtual ~message_serialization_service() {}

public:
  virtual const buffer_type& serialize() = 0;
};

message_serialization_service::pointer_type
make_message_serialization_service(const data_group_type& group);
message_serialization_service::pointer_type
make_message_serialization_service(bool positive);

class message_unserialization_service
{
public:
  typedef std::shared_ptr<message_unserialization_service> pointer_type;

public:
  virtual ~message_unserialization_service() {}

public:
  virtual bool unserialize(const uint8_t* buffer) = 0;
};

message_unserialization_service::pointer_type
make_message_unserialization_service(bool positive);

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_MESSAGE_SERVICE_H
