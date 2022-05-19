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
  typedef acceptor::pointer_type acceptor_type;
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
  virtual void on_accept(const connection_type conn) {}
  virtual void on_close(const connection_type conn) {}
  virtual void on_read(const connection_type conn,
                       const data_group_type& datas) {}
  virtual void on_write_raw(const connection_type conn,
                            const buffer_type& buffer,
                            std::size_t bytes) {}
  virtual void on_read_raw(const connection_type conn,
                           const buffer_type& buffer,
                           std::size_t bytes) {}

public:
  void run();
  void stop();

private:
  void accept_handler(connection_type conn);
  void read_handler(connection_type conn,
                    const buffer_type& buffer,
                    std::size_t bytes);
  void write_handler(connection_type conn,
                     std::size_t bytes);
  void close_handler(connection_type conn);
  void release_handler(connection_type conn);

private:
  void release_connection(connection_type conn);
  const connection_type get_connection(const session_type session);
  const session_type get_session(const connection_type conn);

private:
  acceptor_type acceptor_;
  bimap_type connection_container_;
  buffer_type read_buffer_;
  buffer_type write_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_API_DEMO_SERVER_H
