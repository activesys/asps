// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
#define ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H

#include <cstring>
#include <list>
#include <vector>
#include <memory>
#include <arpa/inet.h>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

// demo_message without compression
class demo_message
{
public:
  typedef std::shared_ptr<demo_message> pointer_type;

protected:
  typedef std::vector<uint8_t> buffer_type;
  typedef std::vector<demo_data::pointer_type> data_group_type;

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
    attr_none = 0x00,
    attr_same_type = 0x01,
    attr_key_sequence = 0x02,
    attr_same_timestamp = 0x04
  };

public:
  template <typename IT>
  demo_message(IT first, IT second)
    : datas_{{first, second}}
  {}
  virtual ~demo_message() {}

public:
  const buffer_type& serialize();

protected:
  // group
  virtual void group();
  // length
  virtual std::size_t serialization_header_length(data_group_type& group);
  virtual std::size_t serialization_mutable_length(data_group_type& group);
  virtual std::size_t serialization_datas_length(data_group_type& group);
  // decode header
  virtual void serialize_header(data_group_type& group, uint8_t*& pos);
  virtual void serialize_header_flag(data_group_type& group, uint8_t*& pos);
  virtual void serialize_header_length(data_group_type& group, uint8_t*& pos);
  virtual void serialize_header_count(data_group_type& group, uint8_t*& pos);
  virtual void serialize_header_attribute(data_group_type& group, uint8_t*& pos);
  // decode mutable
  virtual void serialize_mutable(data_group_type& group, uint8_t*& pos);
  virtual void serialize_mutable_type(data_group_type& group, uint8_t*& pos);
  virtual void serialize_mutable_key(data_group_type& group, uint8_t*& pos);
  virtual void serialize_mutable_timestamp(data_group_type& group, uint8_t*& pos);
  // decode datas
  virtual void serialize_datas(data_group_type& group, uint8_t*& pos);
  virtual void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos);
  virtual void serialize_data_type(demo_data::pointer_type& data, uint8_t*& pos);
  virtual void serialize_data_key(demo_data::pointer_type& data, uint8_t*& pos);
  virtual void serialize_data_timestamp(demo_data::pointer_type& data, uint8_t*& pos);
  virtual void serialize_data_value(demo_data::pointer_type& data, uint8_t*& pos);

protected:
  std::list<data_group_type> datas_;
  buffer_type buffer_;
};

// demo_message with same type compression
class demo_message_same_type_compression : public demo_message
{
public:
  template <typename IT>
  demo_message_same_type_compression(IT first, IT second)
    : demo_message(first, second)
  {}

public:
  virtual void group() override;
  virtual std::size_t serialization_mutable_length(data_group_type& group) override;
  virtual std::size_t serialization_datas_length(data_group_type& group) override;
  virtual void serialize_header_attribute(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable_type(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos) override;
};

// demo_message with key sequence compression
class demo_message_key_sequence_compression : public demo_message
{
public:
  template <typename IT>
  demo_message_key_sequence_compression(IT first, IT second)
    : demo_message(first, second)
  {}

public:
  virtual void group() override;
  virtual std::size_t serialization_mutable_length(data_group_type& group) override;
  virtual std::size_t serialization_datas_length(data_group_type& group) override;
  virtual void serialize_header_attribute(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable_key(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos) override;

private:
  struct compare
  {
    bool operator()(demo_data::pointer_type& first,
                    demo_data::pointer_type& second)
    {
      return first->key() < second->key();
    }
  };
};

// demo_message with same timestamp compression
class demo_message_same_timestamp_compression : public demo_message
{
public:
  template <typename IT>
  demo_message_same_timestamp_compression(IT first, IT second)
    : demo_message(first, second)
  {}

public:
  virtual void group() override;
  virtual std::size_t serialization_mutable_length(data_group_type& group) override;
  virtual std::size_t serialization_datas_length(data_group_type& group) override;
  virtual void serialize_header_attribute(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_mutable_timestamp(data_group_type& group, uint8_t*& pos) override;
  virtual void serialize_one_data(demo_data::pointer_type& data, uint8_t*& pos) override;
};

template <typename IT>
demo_message::pointer_type
make_demo_message(IT first,
                  IT second,
                  bool compress_same_type = false,
                  bool compress_same_timestamp = false,
                  bool compress_key_sequence = false)
{
  if (compress_same_type) {
    return std::make_shared<demo_message_same_type_compression>(first, second);
  } else if (compress_same_timestamp) {
    return std::make_shared<demo_message_same_timestamp_compression>(first, second);
  } else if (compress_key_sequence) {
    return std::make_shared<demo_message_key_sequence_compression>(first, second);
  } else {
    return std::make_shared<demo_message>(first, second);
  }
}

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_MESSAGE_H
