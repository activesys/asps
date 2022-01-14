// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Message.

#ifndef ASPS_CHAT_MESSAGE_CHATMSG_H
#define ASPS_CHAT_MESSAGE_CHATMSG_H

#include <cstdint>
#include <string>
#include <vector>

namespace asps {
namespace chat {

// Chat Message
class chat_msg
{
  enum {
    chat_id_field_length = 2,
    msg_length_field_length = 2
  };

public:
  chat_msg()
    : chat_id_(0)
  {}
  chat_msg(uint16_t chat_id, const std::string& msg)
    : chat_id_(chat_id),
      msg_(msg)
  {}

public:
  const uint16_t id() const {return chat_id_;}
  void id(uint16_t id) {chat_id_ = id;}
  const std::string& msg() const {return msg_;}
  void msg(const std::string& msg) {msg_ = msg;}

  uint8_t* serialize();
  std::size_t serialized_size();

public:
  static uint16_t header_size()
  {return chat_id_field_length + msg_length_field_length;}

  static uint16_t unserialize_header(std::vector<uint8_t>& buffer);
  static chat_msg unserialize(std::vector<uint8_t>& buffer);

private:
  uint16_t chat_id_;
  std::string msg_;
  std::vector<uint8_t> buffer_;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_MESSAGE_CHATMSG_H
