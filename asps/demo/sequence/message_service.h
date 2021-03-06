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

enum header_field_length {
  header_flag_field_length = 8,
  header_length_field_length = 2,
  header_count_field_length = 2,
  header_attribute_field_length = 1
};
enum mutable_field_length {
  mutable_type_field_length = 1,
  mutable_key_field_length = 4,
  mutable_timestamp_field_length = 8
};
enum data_field_length {
  data_type_field_length = 1,
  data_key_field_length = 4,
  data_timestamp_field_length = 8
};
enum attribute {
  attr_none = 0x00,
  attr_same_type = 0x01,
  attr_key_sequence = 0x02,
  attr_same_timestamp = 0x04
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

message_type get_message_type(const buffer_type& buffer);

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

class message_unserialization_service
{
public:
  typedef std::shared_ptr<message_unserialization_service> pointer_type;

public:
  virtual ~message_unserialization_service() {}

public:
  const data_group_type& datas()
  {return datas_;}

public:
  virtual bool unserialize(buffer_type& buffer) = 0;

protected:
  data_group_type datas_;
};

// Make function for serialization and unserialization service
message_serialization_service::pointer_type
make_client_data_message(const data_group_type& group);
message_serialization_service::pointer_type
make_client_positive_keepalive();
message_serialization_service::pointer_type
make_client_negative_keepalive_ack();
message_unserialization_service::pointer_type
make_client_positive_keepalive_ack();
message_unserialization_service::pointer_type
make_client_negative_keepalive();
message_unserialization_service::pointer_type
make_server_data_message();
message_unserialization_service::pointer_type
make_server_positive_keepalive();
message_serialization_service::pointer_type
make_server_positive_keepalive_ack();
message_serialization_service::pointer_type
make_server_negative_keepalive();
message_unserialization_service::pointer_type
make_server_negative_keepalive_ack();
message_unserialization_service::pointer_type
make_invalid_message();

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_MESSAGE_SERVICE_H
