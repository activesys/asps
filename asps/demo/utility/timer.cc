// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo timer.

#include <functional>
#include <asps/demo/utility/timer.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

timer_service::pointer_type
make_timer_service(uint32_t expiry, timer_service::timeout_handler timeout)
{
  return std::make_shared<timer>(*context, expiry, timeout);
}

void timer::start()
{
  timer_.expires_after(expiry_);
  timer_.async_wait(std::bind(&timer::on_timeout, this, _1));
}

void timer::stop()
{
  timer_.cancel();
}

void timer::on_timeout(const error_code& ec)
{
  if (!ec) {
    handler_();
  }
}

} // demo
} // asps
