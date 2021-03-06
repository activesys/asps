// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#include <algorithm>
#include <map>
#include <asps/demo/message/client_message.h>
#include <asps/demo/message/keepalive.h>
#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

message_type get_message_type(const buffer_type& buffer)
{
  std::size_t length = 0;

  if (buffer.empty()) {
    return invalid_message;
  }

  length = std::min(sizeof(DATA_MESSAGE_FLAG), buffer.size());
  if (memcmp(buffer.data(),
             DATA_MESSAGE_FLAG,
             length) == 0) {
    return data_message;
  }

  length = std::min(sizeof(POSITIVE_KEEPALIVE_MESSAGE_FLAG), buffer.size());
  if (memcmp(buffer.data(),
             POSITIVE_KEEPALIVE_MESSAGE_FLAG,
             length) == 0) {
    return positive_keepalive_message;
  }

  if (buffer[0] == config::pack()) {
    return positive_keepalive_ack_message;
  }

  if (buffer[0] == config::nkeep()) {
    return negative_keepalive_message;
  }

  length = std::min(sizeof(NEGATIVE_KEEPALIVE_ACK_MESSAGE_FLAG), buffer.size());
  if (memcmp(buffer.data(),
             NEGATIVE_KEEPALIVE_ACK_MESSAGE_FLAG,
             length) == 0) {
    return negative_keepalive_ack_message;
  }

  return invalid_message;
}

message_serialization_service::pointer_type
make_client_data_message(const data_group_type& group)
{
  return std::make_shared<client_data>(group);
}

message_serialization_service::pointer_type
make_client_positive_keepalive()
{
  return std::make_shared<client_positive_keepalive>();
}

message_serialization_service::pointer_type
make_client_negative_keepalive_ack()
{
  return std::make_shared<client_negative_keepalive_ack>();
}

message_unserialization_service::pointer_type
make_client_positive_keepalive_ack()
{
  return std::make_shared<client_positive_keepalive_ack>();
}

message_unserialization_service::pointer_type
make_client_negative_keepalive()
{
  return std::make_shared<client_negative_keepalive>();
}

message_unserialization_service::pointer_type
make_invalid_message()
{
  return std::make_shared<invalid_unserialization_message>();
}

const buffer_type& client_data::serialize()
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

void client_data::group()
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

void client_data::group_according_to_same_type()
{
  if (config::same_type()) {
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

void client_data::group_according_to_key_sequence()
{
  if (config::key_sequence()) {
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

      datas_.splice(datas_.end(), group_datas);
    }
  }
}

void client_data::group_according_to_same_timestamp()
{
  if (config::same_timestamp()) {
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

std::size_t client_data::serialization_length(data_group_type& group)
{
  return serialization_header_length(group) +
         serialization_mutable_length(group) +
         serialization_datas_length(group);
}

std::size_t client_data::serialization_header_length(data_group_type& group)
{
  return header_flag_field_length +
         header_length_field_length +
         header_count_field_length +
         header_attribute_field_length;
}

std::size_t client_data::serialization_mutable_length(data_group_type& group)
{
  std::size_t length = 0;

  if (config::same_type()) {
    length += mutable_type_field_length;
  }
  if (config::key_sequence()) {
    length += mutable_key_field_length;
  }
  if (config::same_timestamp()) {
    length += mutable_timestamp_field_length;
  }

  return length;
}

std::size_t client_data::serialization_datas_length(data_group_type& group)
{
  std::size_t length = 0;

  /*
   * When all three attributes are set and the current group
   * contains data of type bool, bit compression is required.
   */
  if (group.front()->type() == demo_data::boolean_type &&
      config::same_type() &&
      config::key_sequence() &&
      config::same_timestamp()) {
    length = (group.size() + 7) / 8;
  } else {
    for (auto& data : group) {
      if (!config::same_type()) {
        length += data_type_field_length;
      }
      if (!config::key_sequence()) {
        length += data_key_field_length;
      }
      if (!config::same_timestamp()) {
        length += data_timestamp_field_length;
      }
      length += data->size();
    }
  }

  return length;
}

// deocde header
void client_data::serialize_header(data_group_type& group,
                                    uint8_t*& pos)
{
  serialize_header_flag(group, pos);
  serialize_header_length(group, pos);
  serialize_header_count(group, pos);
  serialize_header_attribute(group, pos);
}

void client_data::serialize_header_flag(data_group_type& group,
                                         uint8_t*& pos)
{
  // flag field is "DEMOV100"
  memcpy(pos,
         DATA_MESSAGE_FLAG,
         header_flag_field_length);
  pos += header_flag_field_length;
}

void client_data::serialize_header_length(data_group_type& group,
                                           uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(serialization_length(group));
  pos += header_length_field_length;
}

void client_data::serialize_header_count(data_group_type& group,
                                          uint8_t*& pos)
{
  *reinterpret_cast<uint16_t*>(pos) = htons(group.size());
  pos += header_count_field_length;
}

void client_data::serialize_header_attribute(data_group_type& group,
                                              uint8_t*& pos)
{
  *pos = attr_none;
  if (config::same_type()) {
    *pos |= attr_same_type;
  }
  if (config::key_sequence()) {
    *pos |= attr_key_sequence;
  }
  if (config::same_timestamp()) {
    *pos |= attr_same_timestamp;
  }
  pos += header_attribute_field_length;
}

// decode mutable
void client_data::serialize_mutable(data_group_type& group,
                                     uint8_t*& pos)
{
  serialize_mutable_type(group, pos);
  serialize_mutable_key(group, pos);
  serialize_mutable_timestamp(group, pos);
}

void client_data::serialize_mutable_type(data_group_type& group,
                                          uint8_t*& pos)
{
  if (config::same_type()) {
    *pos = group.front()->type();
    pos += mutable_type_field_length;
  }
}

void client_data::serialize_mutable_key(data_group_type& group,
                                         uint8_t*& pos)
{
  if (config::key_sequence()) {
    *reinterpret_cast<uint32_t*>(pos) = htonl(group.front()->key());
    pos += mutable_key_field_length;
  }
}

void client_data::serialize_mutable_timestamp(data_group_type& group,
                                               uint8_t*& pos)
{
  if (config::same_timestamp()) {
    *reinterpret_cast<uint64_t*>(pos) = htonll(group.front()->timestamp());
    pos += mutable_timestamp_field_length;
  }
}

// decode datas
void client_data::serialize_datas(data_group_type& group,
                                   uint8_t*& pos)
{
  /*
   * When all three attributes are set and the current group
   * contains data of type bool, bit compression is required.
   */
  if (group.front()->type() == demo_data::boolean_type &&
      config::same_type() &&
      config::key_sequence() &&
      config::same_timestamp()) {
    std::size_t byte_count = (group.size() + 7) / 8;
    for (std::size_t i = 0; i < group.size(); ++i) {
      uint8_t value;
      group.at(i)->serialize_network_sequence_value(&value);
      pos[i/8] |= value << i % 8;
    }
  } else {
    for (auto& data : group) {
      serialize_one_data(data, pos);
    }
  }
}

void client_data::serialize_one_data(demo_data::pointer_type& data,
                                      uint8_t*& pos)
{
  serialize_data_type(data, pos);
  serialize_data_key(data, pos);
  serialize_data_timestamp(data, pos);
  serialize_data_value(data, pos);
}

void client_data::serialize_data_type(demo_data::pointer_type& data,
                                       uint8_t*& pos)
{
  if (!config::same_type()) {
    *pos = data->type();
    pos += data_type_field_length;
  }
}

void client_data::serialize_data_key(demo_data::pointer_type& data,
                                      uint8_t*& pos)
{
  if (!config::key_sequence()) {
    *reinterpret_cast<uint32_t*>(pos) = htonl(data->key());
    pos += data_key_field_length;
  }
}

void client_data::serialize_data_timestamp(demo_data::pointer_type& data,
                                            uint8_t*& pos)
{
  if (!config::same_timestamp()) {
    *reinterpret_cast<uint64_t*>(pos) = htonll(data->timestamp());
    pos += data_timestamp_field_length;
  }
}

void client_data::serialize_data_value(demo_data::pointer_type& data,
                                        uint8_t*& pos)
{
  data->serialize_network_sequence_value(pos);
  pos += data->size();
}

} // demo
} // asps
