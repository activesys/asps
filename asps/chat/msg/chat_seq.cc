// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Sequence.

#include <asps/chat/msg/chat_seq.h>

using namespace asps::chat;

// Chat Client Sequence
chat_msg& client_seq::get_chat_msg(const std::string& msg)
{
  msg_.id(chat_id_);
  msg_.msg(msg);

  return msg_;
}

void client_seq::set_chat_msg(const chat_msg& msg)
{
  notify_.notify_msg(msg.id(), msg.msg());
}
