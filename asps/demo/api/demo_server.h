// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server.

#ifndef ASPS_DEMO_API_DEMO_SERVER_H
#define ASPS_DEMO_API_DEMO_SERVER_H

#include <string>
#include <asps/demo/api/session_service.h>
#include <asps/demo/api/transport_service.h>
#include <asps/demo/utility/utility.h>

namespace asps {
namespace demo {

class demo_server : public server_observer
{
public:
  demo_server(uint16_t port);
  demo_server(const std::string& ip, uint16_t port);
  virtual ~demo_server() {}

public:
  virtual void update_data(const data_group_type& datas) override;
  virtual void update_send(const buffer_type& buffer) override;
  virtual void update_event() override;

public:
  virtual void on_accept() = 0;
  virtual void on_read(const data_group_type& datas) = 0;
  virtual void on_write(std::size_t bytes) = 0;
  virtual void on_read_raw(const buffer_type& buffer) = 0;

public:
  void run();
  void stop();

private:
  void accept_handler(bool success);
  void read_handler(bool success, const buffer_type& buffer, std::size_t bytes);
  void write_handler(bool success, std::size_t bytes);

private:
  server_transport_service::pointer_type transport_;
  server_session_service::pointer_type session_;
  buffer_type read_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_SERVER_H
