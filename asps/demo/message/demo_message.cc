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
    length += serialization_length(group);
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
  /*
   * The order is determined by the length of the compressed data.
   * The same timestamp attribute compresses 8 bytes, the key sequence
   * attribute compresses 4 bytes, and the same Type attribute
   * compresses 1 byte.
   */
  group_according_to_same_timestamp();
  group_according_to_key_sequence();
  group_according_to_same_type();
}

void demo_message::group_according_to_same_type()
{
  if (same_type_) {
    data_groups_type temp_groups(std::move(datas_));

    for (auto& temp_group : temp_groups) {
      std::map<uint32_t, data_group_type> group_datas;
      for (auto& data : temp_group) {
        group_datas[data->type()].emplace_back(data);
      }

      for (auto& group : group_datas) {
        datas_.emplace_back(group.second);
      }
    }
  }
}

void demo_message::group_according_to_key_sequence()
{
  if (key_sequence_) {
    compare comp;
    for (auto& group : datas_) {
      std::sort(group.begin(), group.end(), comp);
    }

    data_groups_type temp_groups(std::move(datas_));
    for (auto& temp_group : temp_groups) {
      data_groups_type group_datas;
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
}

void demo_message::group_according_to_same_timestamp()
{
  if (same_timestamp_) {
    data_groups_type temp_groups(std::move(datas_));

    for (auto& temp_group : temp_groups) {
      std::map<uint64_t, data_group_type> group_datas;
      for (auto& data : temp_group) {
        group_datas[data->timestamp()].emplace_back(data);
      }

      for (auto& group : group_datas) {
        datas_.emplace_back(group.second);
      }
    }
  }
}

std::size_t demo_message::serialization_length(data_group_type& group)
{
  return serialization_header_length(group) +
         serialization_mutable_length(group) +
         serialization_datas_length(group);
}

std::size_t demo_message::serialization_header_length(data_group_type& group)
{
  return header_flag_field_length +
         header_length_field_length +
         header_count_field_length +
         header_attribute_field_length;
}

std::size_t demo_message::serialization_mutable_length(data_group_type& group)
{
  std::size_t length = 0;

  if (same_type_) {
    length += mutable_type_field_length;
  }
  if (key_sequence_) {
    length += mutable_key_field_length;
  }
  if (same_timestamp_) {
    length += mutable_timestamp_field_length;
  }

  return length;
}

std::size_t demo_message::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  for (auto& data : group) {
    if (!same_type_) {
      length += data_type_field_length;
    }
    if (!key_sequence_) {
      length += data_key_field_length;
    }
    if (!same_timestamp_) {
      length += data_timestamp_field_length;
    }
    length += data->size();
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
  memcpy(reinterpret_cast<char*>(pos), "DEMOV100", header_flag_field_length);
  pos += header_flag_field_length;
}

void demo_message::serialize_header_length(data_group_type& group,
                                           uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(serialization_length(group));
  pos += header_length_field_length;
}

void demo_message::serialize_header_count(data_group_type& group,
                                          uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(group.size());
  pos += header_count_field_length;
}

void demo_message::serialize_header_attribute(data_group_type& group,
                                              uint8_t*& pos)
{
  *pos = attr_none;
  if (same_type_) {
    *pos |= attr_same_type;
  }
  if (key_sequence_) {
    *pos |= attr_key_sequence;
  }
  if (same_timestamp_) {
    *pos |= attr_same_timestamp;
  }
  pos += header_attribute_field_length;
}

// decode mutable
void demo_message::serialize_mutable(data_group_type& group,
                                     uint8_t*& pos)
{
  serialize_mutable_type(group, pos);
  serialize_mutable_key(group, pos);
  serialize_mutable_timestamp(group, pos);
}

void demo_message::serialize_mutable_type(data_group_type& group,
                                          uint8_t*& pos)
{
  if (same_type_) {
    *pos = group.front()->type();
    pos += mutable_type_field_length;
  }
}

void demo_message::serialize_mutable_key(data_group_type& group,
                                         uint8_t*& pos)
{
  if (key_sequence_) {
    *reinterpret_cast<uint32_t*>(pos) = htonl(group.front()->key());
    pos += mutable_key_field_length;
  }
}

void demo_message::serialize_mutable_timestamp(data_group_type& group,
                                               uint8_t*& pos)
{
  if (same_timestamp_) {
    *reinterpret_cast<uint64_t*>(pos) = htonll(group.front()->timestamp());
    pos += mutable_timestamp_field_length;
  }
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
  if (!same_type_) {
    *pos = data->type();
    pos += data_type_field_length;
  }
}

void demo_message::serialize_data_key(demo_data::pointer_type& data,
                                      uint8_t*& pos)
{
  if (!key_sequence_) {
    *reinterpret_cast<uint32_t*>(pos) = htonl(data->key());
    pos += data_key_field_length;
  }
}

void demo_message::serialize_data_timestamp(demo_data::pointer_type& data,
                                            uint8_t*& pos)
{
  if (!same_timestamp_) {
    *reinterpret_cast<uint64_t*>(pos) = htonll(data->timestamp());
    pos += data_timestamp_field_length;
  }
}

void demo_message::serialize_data_value(demo_data::pointer_type& data,
                                        uint8_t*& pos)
{
  data->assign_network_sequence_value(pos);
  pos += data->size();
}

} // demo
} // asps
