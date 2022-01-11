// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Sequence.

#ifndef ASPS_CHAT_MSG_CHATSEQ_H
#define ASPS_CHAT_MSG_CHATSEQ_H

#include <cstdint>
#include <string>
#include <memory>
#include <functional>

#include <asps/chat/notify.h>
#include <asps/chat/msg/chat_msg.h>

namespace asps {
namespace chat {

// Chat Client Sequence
class client_seq
{
public:
  client_seq(uint16_t chat_id, client_notify& notify)
    : chat_id_(chat_id),
      notify_(notify)
  {}

public:
  chat_msg& get_chat_msg(const std::string& msg);
  void set_chat_msg(const chat_msg& msg);

private:
  uint16_t chat_id_;
  chat_msg msg_;
  client_notify& notify_;
};

typedef std::shared_ptr<client_seq> client_seq_ptr;

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_MSG_CHATSEQ_H
