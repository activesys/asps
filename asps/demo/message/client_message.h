// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Client Message.

#ifndef ASPS_DEMO_MESSAGE_CLIENT_MESSAGE_H
#define ASPS_DEMO_MESSAGE_CLIENT_MESSAGE_H

#include <cstdint>
#include <cstring>
#include <list>
#include <vector>
#include <memory>
#include <arpa/inet.h>
#include <asps/demo/sequence/message_service.h>

namespace asps {
namespace demo {

// client_data without compression
class client_data : public message_serialization_service
{
  typedef std::list<data_group_type> data_groups_type;

  // for sort demo_data
  struct compare
  {
    bool operator()(demo_data::pointer_type& first,
                    demo_data::pointer_type& second)
    {
      return first->key() < second->key();
    }
  };

public:
  client_data(const data_group_type& group)
    : message_serialization_service(),
      datas_{group}
  {}

public:
  const buffer_type& serialize() override;

private:
  // group
  void group();
  void group_according_to_same_type();
  void group_according_to_key_sequence();
  void group_according_to_same_timestamp();
  // length
   std::size_t serialization_length(data_group_type& group);
   std::size_t serialization_header_length(data_group_type& group);
   std::size_t serialization_mutable_length(data_group_type& group);
   std::size_t serialization_datas_length(data_group_type& group);
  // decode header
   void serialize_header(data_group_type& group, uint8_t*& pos);
   void serialize_header_flag(data_group_type& group, uint8_t*& pos);
   void serialize_header_length(data_group_type& group, uint8_t*& pos);
   void serialize_header_count(data_group_type& group, uint8_t*& pos);
   void serialize_header_attribute(data_group_type& group, uint8_t*& pos);
  // decode mutable
   void serialize_mutable(data_group_type& group, uint8_t*& pos);
   void serialize_mutable_type(data_group_type& group, uint8_t*& pos);
   void serialize_mutable_key(data_group_type& group, uint8_t*& pos);
   void serialize_mutable_timestamp(data_group_type& group, uint8_t*& pos);
  // decode datas
   void serialize_datas(data_group_type& group, uint8_t*& pos);
   void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos);
   void serialize_data_type(demo_data::pointer_type& data, uint8_t*& pos);
   void serialize_data_key(demo_data::pointer_type& data, uint8_t*& pos);
   void serialize_data_timestamp(demo_data::pointer_type& data, uint8_t*& pos);
   void serialize_data_value(demo_data::pointer_type& data, uint8_t*& pos);

private:
  data_groups_type datas_;
  buffer_type buffer_;
};

class invalid_unserialization_message : public message_unserialization_service
{
public:
  bool unserialize(buffer_type& buffer) override
  {
    if (!buffer.empty()) {
      buffer.erase(buffer.begin());
      return true;
    } else {
      return false;
    }
  }
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_CLIENT_MESSAGE_H
