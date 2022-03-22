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
  enum value_type {
    boolean_type = 0,
    int8_type = 1,
    uint8_type = 2,
    int16_type = 3,
    uint16_type = 4,
    int32_type = 5,
    uint32_type = 6,
    int64_type = 7,
    uint64_type = 8,
    float32_type = 10,
    float64_type = 11
  };

public:
  demo_message(const demo_data<T>& data)
    : data_(data)
  {}

public:
  const buffer_type& serialize()
  {
    // The size of the entire packet is the size 
    // of the header plus the size of a packet.
    std::size_t header_length = flag_field_length + 
                                length_field_length +
                                count_field_length +
                                attribute_field_length;
    std::size_t value_length = type_field_length +
                               key_field_length +
                               timestamp_field_length +
                               sizeof(uint32_t);
    // decode demo_value
    buffer_.resize(header_length + value_length);
    uint8_t* pos = buffer_.data();
    // decode header, flag field is "DEMO"
    *(pos++) = 0x44;
    *(pos++) = 0x45;
    *(pos++) = 0x4d;
    *(pos++) = 0x4f;
    // decode length field
    *reinterpret_cast<uint16_t*>(pos) = htons(header_length + value_length);
    pos += length_field_length;
    // decode count field
    *reinterpret_cast<uint16_t*>(pos) = htons(1);
    pos += count_field_length;
    // decode attribute field
    *reinterpret_cast<uint32_t*>(pos) = 0;
    pos += attribute_field_length;
    // decode value
    // decode type and key field
    *reinterpret_cast<uint32_t*>(pos) = htonl(uint32_type << 24 | data_.key);
    pos += type_field_length + key_field_length;
    // decode timestamp field
    *reinterpret_cast<uint64_t*>(pos) = data_.timestamp;
    uint64_t low_part = uint64_t(htonl(*reinterpret_cast<uint32_t*>(pos))) << 32;
    uint64_t high_part = htonl(*reinterpret_cast<uint32_t*>(pos + 4));
    *reinterpret_cast<uint64_t*>(pos) = high_part | low_part ;
    pos += timestamp_field_length;
    // decode value field
    *reinterpret_cast<uint32_t*>(pos) = htonl(data_.value);
  
    return buffer_;
  }

private:
  demo_data<T> data_;
  buffer_type buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
