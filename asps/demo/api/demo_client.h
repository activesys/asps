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
#include <asps/transport/transport_service.h>
#include <asps/utility/timer_service.h>
#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

using namespace asps::transport;

class demo_client : public client_observer
{
  typedef timer_service::pointer_type timer_type;
  typedef connector::pointer_type connector_type;
  typedef connection::pointer_type connection_type;
  typedef client_session_service::pointer_type session_type;

public:
  demo_client(const std::string& ip, uint16_t port)
    : connector_(make_connector(ip, port)),
      t0_(make_timer(config::t0() * 1000,
                     std::bind(&demo_client::t0_timeout, this),
                     true))
  {}
  demo_client(const connector::pointer_type conn)
    : connector_(conn),
      t0_(make_timer(config::t0() * 1000,
                     std::bind(&demo_client::t0_timeout, this),
                     true))
  {}
  virtual ~demo_client()
  {}

public:
  virtual void on_connect(const connection_type conn) {}
  virtual void on_close(const connection_type conn) {}
  virtual void on_write_raw(const connection_type conn,
                            const buffer_type& buffer,
                            std::size_t bytes) {}
  virtual void on_read_raw(const connection_type conn,
                           const buffer_type& buffer,
                           std::size_t bytes) {}

public:
  bool send(const data_group_type& group);
  void close();
  void run();
  void stop();

private:
  void connect_handler(connection_type conn);
  void write_handler(connection_type conn,
                     std::size_t bytes);
  void read_handler(connection_type conn,
                    const buffer_type& buffer,
                    std::size_t bytes);
  void close_handler(connection_type conn);
  void t0_timeout();

private:
  void update_send(const buffer_type& buffer) override;
  void update_event() override;

private:
  connector_type connector_;
  connection_type connection_;
  session_type session_;
  timer_type t0_;
  buffer_type read_buffer_;
  buffer_type write_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_CLIENT_H
