// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Message.

#include <cstring>
#include <netinet/in.h>
#include <asps/chat/message/chat_msg.h>

using namespace asps::chat;

// Chat Message
std::size_t chat_msg::serialized_size()
{
  return chat_id_field_length + msg_length_field_length + msg_.size();
}

uint8_t* chat_msg::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();

  *reinterpret_cast<uint16_t*>(pos) = htons(chat_id_);
  pos += chat_id_field_length;
  *reinterpret_cast<uint16_t*>(pos) = htons(static_cast<uint16_t>(msg_.size()));
  pos += msg_length_field_length;
  memcpy(pos, msg_.data(), msg_.size());

  return buffer_.data();
}

uint16_t chat_msg::unserialize_header(std::vector<uint8_t>& buffer)
{
  uint8_t* pos = buffer.data();
  pos += chat_id_field_length;

  return ntohs(*reinterpret_cast<uint16_t*>(pos));
}

chat_msg chat_msg::unserialize(std::vector<uint8_t>& buffer)
{
  uint8_t* pos = buffer.data();
  uint16_t chat_id = ntohs(*reinterpret_cast<uint16_t*>(pos));
  pos += chat_id_field_length;
  uint16_t msg_len = ntohs(*reinterpret_cast<uint16_t*>(pos));
  pos += msg_length_field_length;
  std::string msg(reinterpret_cast<char*>(pos), msg_len);

  return chat_msg(chat_id, msg);
}
