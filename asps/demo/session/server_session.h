// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Session.

#ifndef ASPS_DEMO_SESSION_SERVER_SESSION_H
#define ASPS_DEMO_SESSION_SERVER_SESSION_H

#include <asps/demo/api/session_service.h>
#include <asps/demo/session/sequence_service.h>
#include <asps/utility/timer_service.h>

namespace asps {
namespace demo {

using namespace asps::utility;

// Server Session
class server_session
  : public server_session_service,
    public data_observer,
    public event_observer
{
public:
  server_session();
  ~server_session();

public:
  virtual void receive(buffer_type& buffer) override;

private:
  virtual void update_data(const data_group_type& datas) override;
  virtual void update_event() override;

private:
  bool receive_one_package(buffer_type& buffer);
  void t1_timeout();

private:
  data_receive_sequence_service::pointer_type data_sequence_;
  active_sequence_service::pointer_type nkeep_sequence_;
  garbage_collector_sequence_service::pointer_type gc_sequence_;
  timer_service::pointer_type t1_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_SERVER_SESSION_H
