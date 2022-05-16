// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client.

#include <algorithm>
#include <functional>
#include <asps/demo/api/demo_client.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

bool demo_client::send(const data_group_type& group)
{
  return session_ && session_->send(group);
}

void demo_client::run()
{
  connector_->set_handler(std::bind(&demo_client::connect_handler, this, _1, _2));
  connector_->connect();
  connector_->run();
}

void demo_client::close()
{
  if (connection_) {
    connection_->close();
  }
  t0_->start();
}

void demo_client::stop()
{
  close();
  t0_->stop();
  connector_->stop();
}

void demo_client::t0_timeout()
{
  connector_->connect();
}

void demo_client::connect_handler(bool success, connection::pointer_type conn)
{
  if (success && conn) {
    connection_ = conn;
    session_ = make_client_session_service();
    session_->register_observer(this);
    t0_->stop();

    connection_->set_handler(std::bind(&demo_client::read_handler, this, _1, _2, _3),
                             std::bind(&demo_client::write_handler, this, _1, _2),
                             std::bind(&demo_client::close_handler, this, _1));
    connection_->read();
  }
  on_connect(success);
}

void demo_client::close_handler(connection::pointer_type conn)
{}

void demo_client::write_handler(connection::pointer_type conn,
                                std::size_t bytes)
{
  on_write(true, bytes);
}

void demo_client::read_handler(connection::pointer_type conn,
                               const buffer_type& buffer,
                               std::size_t bytes)
{
  std::size_t remain_size = read_buffer_.size();
  read_buffer_.resize(remain_size + bytes);
  std::copy(buffer.begin(),
            buffer.begin() + bytes,
            read_buffer_.begin() + remain_size);
  session_->receive(read_buffer_);

  if (connection_) {
    connection_->read();
  }
}

void demo_client::update_send(const buffer_type& buf)
{
  if (connection_) {
    connection_->write(buf);
  }
}

void demo_client::update_event()
{
  close();
}

} // demo
} // asps
