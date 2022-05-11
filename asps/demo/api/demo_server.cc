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
  acceptor_->accept(std::bind(&demo_server::accept_handler,
                              this,
                              std::placeholders::_1));
  acceptor_->run();
}

void demo_server::stop()
{
  acceptor_->stop();
}

void demo_server::accept_handler(connection::pointer_type conn)
{
  session_type session = make_server_session_service();
  session->register_observer(this);
  connection_container_.insert(bimap_type::value_type(conn, session));

  on_accept();
  conn->read(std::bind(&demo_server::read_handler,
                       this,
                       std::placeholders::_1,
                       std::placeholders::_2,
                       std::placeholders::_3));
}

void demo_server::read_handler(connection::pointer_type conn,
                               const buffer_type& buffer,
                               std::size_t bytes)
{
  bimap_type::left_const_iterator it = connection_container_.left.find(conn);
  if (it != connection_container_.left.end()) {
    on_read_raw(buffer);

    std::size_t remain_size = read_buffer_.size();
    read_buffer_.resize(remain_size + bytes);
    std::copy(buffer.begin(),
              buffer.begin() + bytes,
              read_buffer_.begin() + remain_size);
    it->second->receive(read_buffer_);

    conn->read(std::bind(&demo_server::read_handler,
                         this,
                         std::placeholders::_1,
                         std::placeholders::_2,
                         std::placeholders::_3));
  }
}

void demo_server::write_handler(connection::pointer_type conn,
                                std::size_t bytes)
{
  on_write(bytes);
}

void demo_server::update_data(session_type session, const data_group_type& datas)
{
  on_read(datas);
}

void demo_server::update_send(session_type session, const buffer_type& buffer)
{
  bimap_type::right_const_iterator it = connection_container_.right.find(session);
  if (it != connection_container_.right.end()) {
    it->second->write(buffer, std::bind(&demo_server::write_handler,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
  }
}

void demo_server::update_event(session_type session)
{
  bimap_type::right_const_iterator it = connection_container_.right.find(session);
  if (it != connection_container_.right.end()) {
    it->second->close();
    connection_container_.right.erase(session);
  }
}

} // demo
} // asps
