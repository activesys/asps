// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server.

#include <functional>
#include <asps/demo/api/demo_server.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

demo_server::demo_server(const std::string& ip, uint16_t port)
  : transport_(make_server_transport_service(ip, port)),
    session_(make_server_session_service())
{
  session_->register_observer(this);
}

demo_server::demo_server(uint16_t port)
  : transport_(make_server_transport_service(port)),
    session_(make_server_session_service())
{
  session_->register_observer(this);
}

void demo_server::run()
{
  transport_->accept(std::bind(&demo_server::accept_handler, this, _1));
  transport_->run();
}

void demo_server::stop()
{
  transport_->stop();
}

void demo_server::accept_handler(bool success)
{
  if (success) {
    on_accept();
    transport_->read(std::bind(&demo_server::read_handler, this, _1, _2, _3));
  }
}

void demo_server::read_handler(bool success, const buffer_type& buffer, std::size_t bytes)
{
  if (success) {
    std::size_t remain_size = read_buffer_.size();
    read_buffer_.resize(remain_size + bytes);
    std::copy(buffer.begin(),
              buffer.begin() + bytes,
              read_buffer_.begin() + remain_size);
    session_->receive(read_buffer_);
  }

  transport_->read(std::bind(&demo_server::read_handler, this, _1, _2, _3));
}

void demo_server::update_data(const data_group_type& datas)
{
  on_read(datas);
}

} // demo
} // asps
