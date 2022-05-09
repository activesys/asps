// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Server Message.

#include <cstring>
#include <asps/demo/utility/utility.h>
#include <asps/demo/message/server_message.h>
#include <asps/demo/message/keepalive.h>

namespace asps {
namespace demo {

message_unserialization_service::pointer_type
make_server_data_message()
{
  return std::make_shared<server_data>();
}

message_unserialization_service::pointer_type
make_server_positive_keepalive()
{
  return std::make_shared<server_positive_keepalive>();
}

message_serialization_service::pointer_type
make_server_positive_keepalive_ack()
{
  return std::make_shared<server_positive_keepalive_ack>();
}

message_serialization_service::pointer_type
make_server_negative_keepalive()
{
  return std::make_shared<server_negative_keepalive>();
}

message_unserialization_service::pointer_type
make_server_negative_keepalive_ack()
{
  return std::make_shared<server_negative_keepalive_ack>();
}

void server_data::initialize()
{
  datas_.clear();
  type_ = 0;
  key_ = 0;
  timestamp_ = 0;
}

bool server_data::unserialize(buffer_type& buffer)
{
  initialize();

  if (buffer.empty() || buffer.size() < unserialization_header_length()) {
    return false;
  }

  std::size_t offset = 0;
  std::size_t length = 0;
  const uint8_t* pos = buffer.data();

  // unserialize header
  if (!unserialize_header(pos + offset, length) ||
      buffer.size() < length_) {
    return false;
  }
  offset += length;

  // unserialize mutable
  if (!unserialize_mutable(pos + offset, length)) {
    return false;
  }
  offset += length;

  // unserialize datas
  if (!unserialize_datas(pos + offset, length)) {
    return false;
  }
  offset += length;

  // erase unserialized bytes
  buffer.erase(buffer.begin(), buffer.begin() + offset);
  return true;
}

std::size_t server_data::unserialization_header_length()
{
  return header_flag_field_length +
         header_length_field_length +
         header_count_field_length +
         header_attribute_field_length;
}

bool server_data::unserialize_header(const uint8_t* pos, std::size_t& length)
{
  length = 0;

  // decode flag
  if (memcmp(pos, DATA_MESSAGE_FLAG, header_flag_field_length) != 0) {
    return false;
  }
  length += header_flag_field_length;
  pos += header_flag_field_length;

  // decode length
  length_ = ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  length += header_length_field_length;
  pos += header_length_field_length;

  // decode count
  count_ = ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  length += header_count_field_length;
  pos += header_count_field_length;

  // decode attribute
  attribute_ = *pos;
  length += header_attribute_field_length;
  pos += header_attribute_field_length;

  return true;
}

bool server_data::unserialize_mutable(const uint8_t* pos, std::size_t& length)
{
  length = 0;

  // decode type
  if (attribute_ & attr_same_type) {
    type_ = *pos;
    length += mutable_type_field_length;
    pos += mutable_type_field_length;
  }
  // decode key
  if (attribute_ & attr_key_sequence) {
    key_ = ntohl(*reinterpret_cast<uint32_t*>(const_cast<uint8_t*>(pos)));
    length += mutable_key_field_length;
    pos += mutable_key_field_length;
  }
  // decode timestamp
  if (attribute_ & attr_same_timestamp) {
    timestamp_ = ntohll(*reinterpret_cast<uint64_t*>(const_cast<uint8_t*>(pos)));
    length += mutable_timestamp_field_length;
    pos += mutable_timestamp_field_length;
  }

  return true;
}

bool server_data::unserialize_datas(const uint8_t* pos, std::size_t& length)
{
  length = 0;
  for (uint16_t i = 0; i < count_; ++i) {
    std::size_t one_data_length = 0;
    if (!unserialize_one_data(pos, one_data_length, i)) {
      return false;
    }
    length += one_data_length;
    pos += one_data_length;
  }

  return true;
}

bool server_data::unserialize_one_data(const uint8_t* pos,
                                       std::size_t& length,
                                       uint16_t index)
{
  length = 0;

  // decode type
  uint8_t type = 0;
  if (attribute_ & attr_same_type) {
    type = type_;
  } else {
    type = *pos;
    length += data_type_field_length;
    pos += data_type_field_length;
  }

  // decode key
  uint32_t key = 0;
  if (attribute_ & attr_key_sequence) {
    key = key_ + index;
  } else {
    key = ntohl(*reinterpret_cast<uint32_t*>(const_cast<uint8_t*>(pos)));
    length += data_key_field_length;
    pos += data_key_field_length;
  }

  // decode timestamp
  uint64_t timestamp = 0;
  if (attribute_ & attr_same_timestamp) {
    timestamp = timestamp_;
  } else {
    timestamp = ntohll(*reinterpret_cast<uint64_t*>(const_cast<uint8_t*>(pos)));
    length += data_timestamp_field_length;
    pos += data_timestamp_field_length;
  }

  // decode value
  demo_data::pointer_type value;
  switch (type) {
  case demo_data::float64_type:
    value = make_demo_data<double>(key, 0, timestamp);
    break;
  case demo_data::float32_type:
    value = make_demo_data<float>(key, 0, timestamp);
    break;
  case demo_data::uint64_type:
    value = make_demo_data<uint64_t>(key, 0, timestamp);
    break;
  case demo_data::int64_type:
    value = make_demo_data<int64_t>(key, 0, timestamp);
    break;
  case demo_data::uint32_type:
    value = make_demo_data<uint32_t>(key, 0, timestamp);
    break;
  case demo_data::int32_type:
    value = make_demo_data<int32_t>(key, 0, timestamp);
    break;
  case demo_data::uint16_type:
    value = make_demo_data<uint16_t>(key, 0, timestamp);
    break;
  case demo_data::int16_type:
    value = make_demo_data<int16_t>(key, 0, timestamp);
    break;
  case demo_data::uint8_type:
    value = make_demo_data<uint8_t>(key, 0, timestamp);
    break;
  case demo_data::int8_type:
    value = make_demo_data<int8_t>(key, 0, timestamp);
    break;
  case demo_data::boolean_type:
    value = make_demo_data<bool>(key, 0, timestamp);
    break;
  }

  if (value) {
    value->unserialize_network_sequence_value(const_cast<uint8_t*>(pos));
    length += value->size();
    pos += value->size();
    datas_.push_back(value);

    return true;
  } else {
    return false;
  }
}

} // demo
} // asps
