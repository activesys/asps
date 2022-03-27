// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
#define ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H

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
  enum value_field_length {
    type_field_length = 1,
    key_field_length = 4,
    timestamp_field_length = 8
  };

public:
  template <typename IT>
  demo_message(IT first, IT second)
    : datas_(first, second)
  {}

public:
  const buffer_type& serialize()
  {
    buffer_.resize(serialization_header_length() +
                   serialization_datas_length());
    uint8_t* pos = buffer_.data();

    serialize_header(pos);
    serialize_datas(pos);

    return buffer_;
  }

private:
  std::size_t serialization_header_length()
  {
    return flag_field_length + 
           length_field_length +
           count_field_length +
           attribute_field_length;
  }

  void serialize_header(uint8_t*& pos)
  {
    // decode header, flag field is "DEMOV100"
    memcpy(reinterpret_cast<char*>(pos), "DEMOV100", flag_field_length);
    pos += flag_field_length;

    // decode length field
    *reinterpret_cast<uint16_t*>(pos) = htons(serialization_header_length() +
                                              serialization_datas_length());
    pos += length_field_length;

    // decode count field
    *reinterpret_cast<uint16_t*>(pos) = htons(datas_.size());
    pos += count_field_length;

    // decode attribute field
    *pos = 0;
    pos += attribute_field_length;
  }

  std::size_t serialization_datas_length()
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

  void serialize_datas(uint8_t*& pos)
  {
    for (auto& data : datas_) {
      // decode type field
      *pos = data->type();
      pos += type_field_length;
    
      // decode key field
      *reinterpret_cast<uint32_t*>(pos) = htonl(data->key());
      pos += key_field_length;

      // decode timestamp field
      *reinterpret_cast<uint64_t*>(pos) = htonll(data->timestamp());
      pos += timestamp_field_length;

      // decode value field
      data->assign_network_sequence_value(pos);
      pos += data->size();
    }
  }

private:
  std::vector<demo_data::pointer_type> datas_;
  buffer_type buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
