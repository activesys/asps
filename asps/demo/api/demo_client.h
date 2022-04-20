// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client.

#ifndef ASPS_DEMO_API_DEMO_CLIENT_H
#define ASPS_DEMO_API_DEMO_CLIENT_H

#include <cstdint>
#include <string>
#include <functional>
#include <asps/demo/api/session_service.h>
#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/api/transport_service.h>
#include <asps/demo/utility/timer_service.h>
#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

class demo_client : public observer
{
public:
  demo_client(transport_service::pointer_type transport = make_transport_service())
    : transport_(transport),
      t0_(make_timer_service(config::t0(),
                             std::bind(&demo_client::t0_timeout, this)))
  {
    t0_->start();
  }
  virtual ~demo_client()
  {
    t0_->stop();
  }

public:
  virtual void on_connect(bool success) = 0;
  virtual void on_write(bool success, std::size_t bytes) = 0;

public:
  bool send(const data_group_type& group);
  void close();
  void run();
  void stop();

private:
  void connect_handler(bool success);
  void write_handler(bool success, std::size_t bytes);
  void read_handler(bool success, const buffer_type& buffer, std::size_t bytes);
  void t0_timeout();

private:
  void update_send(const buffer_type& buffer) override;
  void update_event() override;

private:
  transport_service::pointer_type transport_;
  session_service::pointer_type session_;
  timer_service::pointer_type t0_;
  buffer_type read_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_CLIENT_H
