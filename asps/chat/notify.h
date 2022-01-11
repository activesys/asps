// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Chat Notify.

#ifndef ASPS_CHAT_NOTIFY_H
#define ASPS_CHAT_NOTIFY_H

#include <cstdint>
#include <string>
#include <functional>

namespace asps {
namespace chat {

typedef std::function<void(uint16_t, const std::string&)> notify_msg_type;
typedef std::function<void(const std::string&)> notify_event_type;

// Client Notify
struct client_notify
{
  notify_msg_type notify_msg;
  notify_event_type notify_event;
};

} // namespace chat
} // namespace asps

#endif // ASPS_CHAT_NOTIFY_H
