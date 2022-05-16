// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server.

#include <functional>
#include <asps/demo/api/demo_server.h>

namespace asps {
namespace demo {

void demo_server::run()
{
  acceptor_->set_handler(std::bind(&demo_server::accept_handler,
                                   this,
                                   std::placeholders::_1),
                         std::bind(&demo_server::release_handler,
                                   this,
                                   std::placeholders::_1));
  acceptor_->accept();
  acceptor_->run();
}

void demo_server::stop()
{
  acceptor_->stop();
}

void demo_server::accept_handler(connection_type conn)
{
  if (conn) {
    conn->set_handler(std::bind(&demo_server::read_handler,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2,
                                std::placeholders::_3),
                      std::bind(&demo_server::write_handler,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2),
                      std::bind(&demo_server::close_handler,
                                this,
                                std::placeholders::_1));

    session_type session = make_server_session_service();
    session->register_observer(this);
    connection_container_.insert(bimap_type::value_type(conn, session));

    on_accept(conn);
    conn->read();
  }
}

void demo_server::read_handler(connection_type conn,
                               const buffer_type& buffer,
                               std::size_t bytes)
{
  session_type session = get_session(conn);
  if (session) {
    on_read_raw(conn, buffer, bytes);

    std::size_t remain_size = read_buffer_.size();
    read_buffer_.resize(remain_size + bytes);
    std::copy(buffer.begin(),
              buffer.begin() + bytes,
              read_buffer_.begin() + remain_size);
    session->receive(read_buffer_);

    conn->read();
  }
}

void demo_server::write_handler(connection_type conn,
                                std::size_t bytes)
{
  on_write_raw(conn, write_buffer_, bytes);
}

void demo_server::close_handler(connection_type conn)
{
  release_connection(conn);
}

void demo_server::release_handler(connection_type conn)
{
  release_connection(conn);
}

void demo_server::release_connection(connection_type conn)
{
  if (get_session(conn)) {
    on_close(conn);
    connection_container_.left.erase(conn);
  }
}

const demo_server::connection_type
demo_server::get_connection(const session_type session)
{
  bimap_type::right_const_iterator it = connection_container_.right.find(session);
  return it != connection_container_.right.end() ? it->second : nullptr;
}

const demo_server::session_type
demo_server::get_session(const connection_type conn)
{
  bimap_type::left_const_iterator it = connection_container_.left.find(conn);
  return it != connection_container_.left.end() ? it->second : nullptr;
}

void demo_server::update_data(session_type session, const data_group_type& datas)
{
  connection_type conn = get_connection(session);
  if (conn) {
    on_read(conn, datas);
  }
}

void demo_server::update_send(session_type session, const buffer_type& buffer)
{
  connection_type conn = get_connection(session);
  if (conn) {
    write_buffer_ = buffer;
    conn->write(write_buffer_);
  }
}

void demo_server::update_event(session_type session)
{
  connection_type conn = get_connection(session);
  if (conn) {
    conn->close();
    acceptor_->release(conn);
  }
}

} // demo
} // asps
