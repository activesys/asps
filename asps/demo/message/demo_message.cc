// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#include <asps/demo/message/demo_message.h>

namespace asps {
namespace demo {

const demo_message::buffer_type& demo_message::serialize()
{
  buffer_.resize(serialization_header_length() +
                 serialization_mutable_length() +
                 serialization_datas_length());
  uint8_t* pos = buffer_.data();

  serialize_header(pos);
  serialize_mutable(pos);
  serialize_datas(pos);

  return buffer_;
}

std::size_t demo_message::serialization_header_length()
{
  return flag_field_length +
         length_field_length +
         count_field_length +
         attribute_field_length;
}

std::size_t demo_message::serialization_mutable_length()
{
  return 0;
}

std::size_t demo_message::serialization_datas_length()
{
  std::size_t length = 0;

  for (auto& data : datas_) {
    length += type_field_length +
              key_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

// deocde header
void demo_message::serialize_header(uint8_t*& pos)
{
  serialize_header_flag(pos);
  serialize_header_length(pos);
  serialize_header_count(pos);
  serialize_header_attribute(pos);
}

void demo_message::serialize_header_flag(uint8_t*& pos)
{
  // flag field is "DEMOV100"
  memcpy(reinterpret_cast<char*>(pos), "DEMOV100", flag_field_length);
  pos += flag_field_length;
}

void demo_message::serialize_header_length(uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(serialization_header_length() +
                                            serialization_mutable_length() +
                                            serialization_datas_length());
  pos += length_field_length;
}

void demo_message::serialize_header_count(uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(datas_.size());
  pos += count_field_length;
}

void demo_message::serialize_header_attribute(uint8_t*& pos)
{
  *pos = attr_none;
  pos += attribute_field_length;
}

// decode mutable
void demo_message::serialize_mutable(uint8_t*& pos)
{
}

void demo_message::serialize_mutable_type(uint8_t*& pos)
{
}

void demo_message::serialize_mutable_timestamp(uint8_t*& pos)
{
}

// decode datas
void demo_message::serialize_datas(uint8_t*& pos)
{
  for (auto& data : datas_) {
    serialize_one_data(data, pos);
  }
}

void demo_message::serialize_one_data(demo_data::pointer_type& data,
                                      uint8_t*& pos)
{
  serialize_data_type(data, pos);
  serialize_data_key(data, pos);
  serialize_data_timestamp(data, pos);
  serialize_data_value(data, pos);
}

void demo_message::serialize_data_type(demo_data::pointer_type& data,
                                       uint8_t*& pos)
{
  *pos = data->type();
  pos += type_field_length;
}

void demo_message::serialize_data_key(demo_data::pointer_type& data,
                                      uint8_t*& pos)
{
  *reinterpret_cast<uint32_t*>(pos) = htonl(data->key());
  pos += key_field_length;
}

void demo_message::serialize_data_timestamp(demo_data::pointer_type& data,
                                            uint8_t*& pos)
{
  *reinterpret_cast<uint64_t*>(pos) = htonll(data->timestamp());
  pos += timestamp_field_length;
}

void demo_message::serialize_data_value(demo_data::pointer_type& data,
                                        uint8_t*& pos)
{
  data->assign_network_sequence_value(pos);
  pos += data->size();
}

// demo_message with same type compression
std::size_t demo_message_same_type_compression::serialization_mutable_length()
{
  return mutable_type_field_length;
}

std::size_t demo_message_same_type_compression::serialization_datas_length()
{
  std::size_t length = 0;

  for (auto& data : datas_) {
    length += key_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

void demo_message_same_type_compression::serialize_header_attribute(uint8_t*& pos)
{
  *pos = attr_same_type;
  pos += attribute_field_length;
}

void demo_message_same_type_compression::serialize_mutable(uint8_t*& pos)
{
  serialize_mutable_type(pos);
}

void demo_message_same_type_compression::serialize_mutable_type(uint8_t*& pos)
{
  *pos = datas_[0]->type();
  pos += mutable_type_field_length;
}

void demo_message_same_type_compression::serialize_one_data(demo_data::pointer_type& data,
                                                            uint8_t*& pos)
{
  serialize_data_key(data, pos);
  serialize_data_timestamp(data, pos);
  serialize_data_value(data, pos);
}

// demo_message with same timestamp compression
std::size_t demo_message_same_timestamp_compression::serialization_mutable_length()
{
  return mutable_timestamp_field_length;
}

std::size_t demo_message_same_timestamp_compression::serialization_datas_length()
{
  std::size_t length = 0;

  for (auto& data : datas_) {
    length += type_field_length +
              key_field_length +
              data->size();
  }

  return length;
}

void demo_message_same_timestamp_compression::serialize_header_attribute(uint8_t*& pos)
{
  *pos = attr_same_timestamp;
  pos += attribute_field_length;
}

void demo_message_same_timestamp_compression::serialize_mutable(uint8_t*& pos)
{
  serialize_mutable_timestamp(pos);
}

void demo_message_same_timestamp_compression::serialize_mutable_timestamp(uint8_t*& pos)
{
  *reinterpret_cast<uint64_t*>(pos) = htonll(datas_[0]->timestamp());
  pos += mutable_timestamp_field_length;
}

void demo_message_same_timestamp_compression::serialize_one_data(demo_data::pointer_type& data,
                                                                 uint8_t*& pos)
{
  serialize_data_type(data, pos);
  serialize_data_key(data, pos);
  serialize_data_value(data, pos);
}

// demo_message with key sequence compression
std::size_t demo_message_key_sequence_compression::serialization_mutable_length()
{
  return mutable_key_field_length;
}

std::size_t demo_message_key_sequence_compression::serialization_datas_length()
{
  std::size_t length = 0;

  for (auto& data : datas_) {
    length += type_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

void demo_message_key_sequence_compression::serialize_header_attribute(uint8_t*& pos)
{
  *pos = attr_key_sequence;
  pos += attribute_field_length;
}

void demo_message_key_sequence_compression::serialize_mutable(uint8_t*& pos)
{
  serialize_mutable_key(pos);
}

void demo_message_key_sequence_compression::serialize_mutable_key(uint8_t*& pos)
{
  *reinterpret_cast<uint32_t*>(pos) = htonl(datas_[0]->key());
  pos += mutable_key_field_length;
}

void demo_message_key_sequence_compression::serialize_one_data(demo_data::pointer_type& data,
                                                               uint8_t*& pos)
{
  serialize_data_type(data, pos);
  serialize_data_timestamp(data, pos);
  serialize_data_value(data, pos);
}

} // demo
} // asps
