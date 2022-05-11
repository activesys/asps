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
#include <boost/bimap.hpp>

namespace asps {
namespace demo {

class demo_server : public server_observer
{
  typedef connection::pointer_type connection_type;
  typedef server_session_service::pointer_type session_type;
  typedef boost::bimap<connection_type, session_type> bimap_type;

public:
  demo_server(uint16_t port)
    : acceptor_(make_acceptor(port))
  {}
  demo_server(const std::string& ip, uint16_t port)
    : acceptor_(make_acceptor(ip, port))
  {}
  demo_server(const acceptor::pointer_type acce)
    : acceptor_(acce)
  {}
  virtual ~demo_server() {}

public:
  virtual void update_data(session_type session, const data_group_type& datas) override;
  virtual void update_send(session_type session, const buffer_type& buffer) override;
  virtual void update_event(session_type session) override;

public:
  virtual void on_accept() = 0;
  virtual void on_read(const data_group_type& datas) = 0;
  virtual void on_write(std::size_t bytes) = 0;
  virtual void on_read_raw(const buffer_type& buffer) = 0;

public:
  void run();
  void stop();

private:
  void accept_handler(connection::pointer_type conn);
  void read_handler(connection::pointer_type conn,
                    const buffer_type& buffer,
                    std::size_t bytes);
  void write_handler(connection::pointer_type conn,
                     std::size_t bytes);

private:
  acceptor::pointer_type acceptor_;
  bimap_type connection_container_;
  buffer_type read_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_SERVER_H
