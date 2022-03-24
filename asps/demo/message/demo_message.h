// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
#define ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H

#include <arpa/inet.h>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

template <typename T>
class demo_message
{
  typedef std::vector<uint8_t> buffer_type;

  enum header_field_length {
    flag_field_length = 4,
    length_field_length = 2,
    count_field_length = 2,
    attribute_field_length = 4
  };
  enum value_field_length {
    type_field_length = 1,
    key_field_length = 3,
    timestamp_field_length = 8
  };

public:
  demo_message(const demo_data<T>& data)
    : data_(data)
  {}

public:
  const buffer_type& serialize()
  {
    buffer_.resize(serialization_header_length() +
                   serialization_value_length());
    uint8_t* pos = buffer_.data();

    serialize_header(pos);
    serialize_value(pos);
  
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
    // decode header, flag field is "DEMO"
    *(pos++) = 0x44;
    *(pos++) = 0x45;
    *(pos++) = 0x4d;
    *(pos++) = 0x4f;

    // decode length field
    *reinterpret_cast<uint16_t*>(pos) = htons(serialization_header_length() +
                                              serialization_value_length());
    pos += length_field_length;

    // decode count field
    *reinterpret_cast<uint16_t*>(pos) = htons(1);
    pos += count_field_length;

    // decode attribute field
    *reinterpret_cast<uint32_t*>(pos) = 0;
    pos += attribute_field_length;
  }

  std::size_t serialization_value_length()
  {
    return type_field_length +
           key_field_length +
           timestamp_field_length +
           data_.size();
  }
  void serialize_value(uint8_t*& pos)
  {
    // decode type and key field
    *reinterpret_cast<uint32_t*>(pos) = htonl(data_.type << 24 | data_.key);
    pos += type_field_length + key_field_length;

    // decode timestamp field
    *reinterpret_cast<uint64_t*>(pos) = htonll(data_.timestamp);
    pos += timestamp_field_length;

    // decode value field
    *reinterpret_cast<T*>(pos) = data_.network_sequence_of_value();
    pos += data_.size();
  }

private:
  demo_data<T> data_;
  buffer_type buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
