// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Session.

#ifndef ASPS_DEMO_SESSION_CLIENT_SESSION_H
#define ASPS_DEMO_SESSION_CLIENT_SESSION_H

#include <cstdint>
#include <vector>

#include <asps/demo/session/sequence_service.h>
#include <asps/demo/utility/timer_service.h>
#include <asps/demo/api/session_service.h>

namespace asps {
namespace demo {

// Demo Client Session
class client_session
  : public client_session_service,
    public event_observer
{
public:
  client_session();
  ~client_session();

public:
  bool send(const data_group_type& group) override;
  void receive(buffer_type& buffer) override;

private:
  void update_event() override;

private:
  bool receive_one_package(buffer_type& buffer);
  void t1_timeout();

private:
  buffer_type buffer_;
  data_send_sequence_service::pointer_type data_sequence_;
  active_sequence_service::pointer_type pkeep_sequence_;
  garbage_collector_sequence_service::pointer_type gc_sequence_;
  timer_service::pointer_type t1_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_CLIENT_SESSION_H
