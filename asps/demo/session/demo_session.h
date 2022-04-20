// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#ifndef ASPS_DEMO_SESSION_DEMO_SESSION_H
#define ASPS_DEMO_SESSION_DEMO_SESSION_H

#include <cstdint>
#include <vector>

#include <asps/demo/session/sequence_service.h>
#include <asps/demo/utility/timer_service.h>
#include <asps/demo/api/session_service.h>

namespace asps {
namespace demo {

// Demo Session
class demo_session : public session_service, public sequence_observer
{
public:
  demo_session();
  ~demo_session();

public:
  bool send(const data_group_type& group) override;
  void receive(buffer_type& buffer) override;

  void t1_timeout();

public:
  void update_event() override;

private:
  bool receive_one_package(buffer_type& buffer);

private:
  buffer_type buffer_;
  sequence_service::pointer_type pkeep_seq_;
  timer_service::pointer_type t1_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_DEMO_SESSION_H
