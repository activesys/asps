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
  transport_->connect(std::bind(&demo_client::connect_handler, this, _1));
  transport_->run();
}

void demo_client::close()
{
  transport_->close();
  t0_->start();
}

void demo_client::stop()
{
  close();
  t0_->stop();
  transport_->stop();
}

void demo_client::t0_timeout()
{
  transport_->connect(std::bind(&demo_client::connect_handler, this, _1));
}

void demo_client::connect_handler(bool success)
{
  if (success) {
    session_ = make_session_service();
    session_->register_observer(this);
    t0_->stop();
    transport_->read(std::bind(&demo_client::read_handler, this, _1, _2, _3));
  }
  on_connect(success);
}

void demo_client::write_handler(bool success, std::size_t bytes)
{
  on_write(success, bytes);
}

void demo_client::read_handler(bool success,
                               const buffer_type& buffer,
                               std::size_t bytes)
{
  if (success) {
    std::size_t remain_size = read_buffer_.size();
    read_buffer_.resize(remain_size + bytes);
    std::copy(buffer.begin(),
              buffer.begin() + bytes,
              read_buffer_.begin() + remain_size);
    session_->receive(read_buffer_);
  }

  transport_->read(std::bind(&demo_client::read_handler, this, _1, _2, _3));
}

void demo_client::update_send(const buffer_type& buf)
{
  transport_->write(buf, std::bind(&demo_client::write_handler, this, _1, _2));
}

void demo_client::update_event()
{
  close();
}

} // demo
} // asps
