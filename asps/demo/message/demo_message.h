// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
#define ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H

#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

class demo_message
{
  typedef std::vector<uint8_t> buffer_type;

  enum header_field_length {
    flag_field_length = 8,
    length_field_length = 2,
    count_field_length = 2,
    attribute_field_length = 1
  };
  enum mutable_field_length {
    mutable_type_field_length = 1,
    mutable_key_field_length = 4,
    mutable_timestamp_field_length = 8
  };
  enum data_field_length {
    type_field_length = 1,
    key_field_length = 4,
    timestamp_field_length = 8
  };
  enum attribute {
    attr_same_type = 0x01,
    attr_key_sequence = 0x02,
    attr_same_timestamp = 0x04
  };

public:
  template <typename IT>
  demo_message(IT first,
               IT second,
               bool compress_same_type = false,
               bool compress_same_timestamp = false)
    : datas_(first, second),
      compress_same_type_(compress_same_type),
      compress_same_timestamp_(compress_same_timestamp)
  {}

public:
  const buffer_type& serialize();

private:
  std::size_t serialization_header_length();
  std::size_t serialization_mutable_length();
  std::size_t serialization_datas_length();
  // decode header
  void serialize_header(uint8_t*& pos);
  void serialize_header_flag(uint8_t*& pos);
  void serialize_header_length(uint8_t*& pos);
  void serialize_header_count(uint8_t*& pos);
  void serialize_header_attribute(uint8_t*& pos);
  // decode mutable
  void serialize_mutable(uint8_t*& pos);
  void serialize_mutable_type(uint8_t*& pos);
  void serialize_mutable_timestamp(uint8_t*& pos);
  // decode datas
  void serialize_datas(uint8_t*& pos);
  void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos);
  void serialize_data_type(demo_data::pointer_type& data, uint8_t*& pos);
  void serialize_data_key(demo_data::pointer_type& data, uint8_t*& pos);
  void serialize_data_timestamp(demo_data::pointer_type& data, uint8_t*& pos);
  void serialize_data_value(demo_data::pointer_type& data, uint8_t*& pos);

private:
  bool compress_same_type_;
  bool compress_same_timestamp_;
  std::vector<demo_data::pointer_type> datas_;
  buffer_type buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
