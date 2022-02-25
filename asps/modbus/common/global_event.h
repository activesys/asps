// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Global Event.

#ifndef ASPS_MODBUS_COMMON_GLOBAL_EVENT_H
#define ASPS_MODBUS_COMMON_GLOBAL_EVENT_H

#include <asps/modbus/api/event.h>

namespace asps {
namespace modbus {

class global_client_event
{
private:
  global_client_event()
    : event_(nullptr)
  {}

public:
  void event(client_event* e) {event_ = e;}
  client_event* event() {return event_;}

public:
  static global_client_event* instance()
  {
    if (!instance_) {
      instance_ = new global_client_event;
    }
    return instance_;
  }

private:
  static global_client_event* instance_;
  client_event* event_;
};

} // modbus
} // asps

#endif // ASPS_MODBUS_COMMON_GLOBAL_EVENT_H
