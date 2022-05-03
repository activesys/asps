// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Session.

#ifndef ASPS_DEMO_SESSION_SERVER_SESSION_H
#define ASPS_DEMO_SESSION_SERVER_SESSION_H

#include <asps/demo/api/session_service.h>
#include <asps/demo/session/sequence_service.h>

namespace asps {
namespace demo {

// Server Session
class server_session
  : public server_session_service,
    public data_observer
{
public:
  server_session();

public:
  virtual void receive(buffer_type& buffer) override;

public:
  virtual void update_data(const data_group_type& datas) override;

private:
  server_data_sequence_service::pointer_type data_sequence_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_SERVER_SESSION_H
