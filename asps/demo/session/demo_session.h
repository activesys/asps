// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#ifndef ASPS_DEMO_SESSION_DEMO_SESSION_H
#define ASPS_DEMO_SESSION_DEMO_SESSION_H

#include <cstdint>
#include <vector>

#include <asps/demo/session/message_service.h>
#include <asps/demo/session/timer_service.h>
#include <asps/demo/api/session_service.h>

namespace asps {
namespace demo {

// Demo Session
class demo_session : public session_service
{
public:
  demo_session();
  ~demo_session();

public:
  buffer_type& serialize_datas(const data_group_type& group) override;
  buffer_type& serialize_keepalive() override;
  bool receive(const uint8_t* buffer) override;

  void t1_timeout();
  void t2_timeout();

private:
  buffer_type buffer_;
  timer_service::pointer_type t1_;
  timer_service::pointer_type t2_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_DEMO_SESSION_H
