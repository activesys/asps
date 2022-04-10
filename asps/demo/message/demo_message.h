// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
#define ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H

#include <cstdint>
#include <cstring>
#include <list>
#include <vector>
#include <memory>
#include <arpa/inet.h>
#include <asps/demo/session/message_service.h>

namespace asps {
namespace demo {

// demo_message without compression
class demo_message : public message_serialization_service
{
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
  demo_message(const data_group_type& group)
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

protected:
  data_groups_type datas_;
  buffer_type buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
