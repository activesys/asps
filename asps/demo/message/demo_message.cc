// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#include <algorithm>
#include <map>
#include <functional>
#include <asps/demo/message/demo_message.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

const demo_message::buffer_type& demo_message::serialize()
{
  group();

  std::size_t length = 0;
  for (auto& group : datas_) {
    length += serialization_header_length(group) +
              serialization_mutable_length(group) +
              serialization_datas_length(group);
  }
  buffer_.resize(length);
  uint8_t* pos = buffer_.data();

  for (auto& group : datas_) {
    serialize_header(group, pos);
    serialize_mutable(group, pos);
    serialize_datas(group, pos);
  }

  return buffer_;
}

void demo_message::group()
{
}

std::size_t demo_message::serialization_header_length(data_group_type& group)
{
  return flag_field_length +
         length_field_length +
         count_field_length +
         attribute_field_length;
}

std::size_t demo_message::serialization_mutable_length(data_group_type& group)
{
  return 0;
}

std::size_t demo_message::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  for (auto& data : group) {
    length += type_field_length +
              key_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

// deocde header
void demo_message::serialize_header(data_group_type& group,
                                    uint8_t*& pos)
{
  serialize_header_flag(group, pos);
  serialize_header_length(group, pos);
  serialize_header_count(group, pos);
  serialize_header_attribute(group, pos);
}

void demo_message::serialize_header_flag(data_group_type& group,
                                         uint8_t*& pos)
{
  // flag field is "DEMOV100"
  memcpy(reinterpret_cast<char*>(pos), "DEMOV100", flag_field_length);
  pos += flag_field_length;
}

void demo_message::serialize_header_length(data_group_type& group,
                                           uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) =
    htons(serialization_header_length(group) +
          serialization_mutable_length(group) +
          serialization_datas_length(group));
  pos += length_field_length;
}

void demo_message::serialize_header_count(data_group_type& group,
                                          uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(group.size());
  pos += count_field_length;
}

void demo_message::serialize_header_attribute(data_group_type& group,
                                              uint8_t*& pos)
{
  *pos = attr_none;
  pos += attribute_field_length;
}

// decode mutable
void demo_message::serialize_mutable(data_group_type& group,
                                     uint8_t*& pos)
{
}

void demo_message::serialize_mutable_type(data_group_type& group,
                                          uint8_t*& pos)
{
}

void demo_message::serialize_mutable_key(data_group_type& group,
                                         uint8_t*& pos)
{
}

void demo_message::serialize_mutable_timestamp(data_group_type& group,
                                               uint8_t*& pos)
{
}

// decode datas
void demo_message::serialize_datas(data_group_type& group,
                                   uint8_t*& pos)
{
  for (auto& data : group) {
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
void demo_message_same_type_compression::group()
{
  std::map<uint32_t, data_group_type> groups;

  for (auto& group : datas_) {
    for (auto& data : group) {
      groups[data->type()].emplace_back(data);
    }
  }

  datas_.clear();
  for (auto& group : groups) {
    datas_.emplace_back(group.second);
  }
}

std::size_t demo_message_same_type_compression::serialization_mutable_length(data_group_type& group)
{
  return mutable_type_field_length;
}

std::size_t demo_message_same_type_compression::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  for (auto& data : group) {
    length += key_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

void demo_message_same_type_compression::serialize_header_attribute(data_group_type& group,
                                                                    uint8_t*& pos)
{
  *pos = attr_same_type;
  pos += attribute_field_length;
}

void demo_message_same_type_compression::serialize_mutable(data_group_type& group,
                                                           uint8_t*& pos)
{
  serialize_mutable_type(group, pos);
}

void demo_message_same_type_compression::serialize_mutable_type(data_group_type& group,
                                                                uint8_t*& pos)
{
  *pos = group.front()->type();
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
void demo_message_same_timestamp_compression::group()
{
  std::map<uint64_t, data_group_type> groups;

  for (auto& group : datas_) {
    for (auto& data : group) {
      groups[data->timestamp()].emplace_back(data);
    }
  }

  datas_.clear();
  for (auto& group : groups) {
    datas_.emplace_back(group.second);
  }
}

std::size_t demo_message_same_timestamp_compression::serialization_mutable_length(data_group_type& group)
{
  return mutable_timestamp_field_length;
}

std::size_t demo_message_same_timestamp_compression::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  for (auto& data : group) {
    length += type_field_length +
              key_field_length +
              data->size();
  }

  return length;
}

void demo_message_same_timestamp_compression::serialize_header_attribute(data_group_type& group,
                                                                         uint8_t*& pos)
{
  *pos = attr_same_timestamp;
  pos += attribute_field_length;
}

void demo_message_same_timestamp_compression::serialize_mutable(data_group_type& group,
                                                                uint8_t*& pos)
{
  serialize_mutable_timestamp(group, pos);
}

void demo_message_same_timestamp_compression::serialize_mutable_timestamp(data_group_type& group,
                                                                          uint8_t*& pos)
{
  *reinterpret_cast<uint64_t*>(pos) = htonll(group.front()->timestamp());
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

void demo_message_key_sequence_compression::group()
{
  compare comp;
  for (auto& group : datas_) {
    std::sort(group.begin(), group.end(), comp);
  }

  std::list<data_group_type> temp_datas(std::move(datas_));
  for (auto& temp_group : temp_datas) {
    std::list<data_group_type> group_datas;
    for (auto& temp_data : temp_group) {
      auto iter = group_datas.begin();
      for (; iter != group_datas.end(); ++iter) {
        if (temp_data->key() == iter->back()->key() + 1) {
          iter->push_back(temp_data);
          break;
        }
      }
      // empty or no match
      if (iter == group_datas.end()) {
        group_datas.push_back({temp_data});
      }
    }

    datas_.merge(std::move(group_datas));
  }
}

std::size_t demo_message_key_sequence_compression::serialization_mutable_length(data_group_type& group)
{
  return mutable_key_field_length;
}

std::size_t demo_message_key_sequence_compression::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  for (auto& data : group) {
    length += type_field_length +
              timestamp_field_length +
              data->size();
  }

  return length;
}

void demo_message_key_sequence_compression::serialize_header_attribute(data_group_type& group,
                                                                       uint8_t*& pos)
{
  *pos = attr_key_sequence;
  pos += attribute_field_length;
}

void demo_message_key_sequence_compression::serialize_mutable(data_group_type& group,
                                                              uint8_t*& pos)
{
  serialize_mutable_key(group, pos);
}

void demo_message_key_sequence_compression::serialize_mutable_key(data_group_type& group,
                                                                  uint8_t*& pos)
{
  *reinterpret_cast<uint32_t*>(pos) = htonl(group.front()->key());
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
