// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo timer.

#include <chrono>
#include <functional>
#include <asps/demo/utility/timer.h>

namespace asps {
namespace demo {

using namespace std::placeholders;

timer_service::pointer_type
make_timer(uint32_t expiry,
           timer_service::timeout_handler timeout,
           bool repeat)
{
  return std::make_shared<timer>(g_context, expiry, timeout, repeat);
}

void timer::start()
{
  if (expiry_ > 0) {
    timer_.expires_after(std::chrono::milliseconds(expiry_));
    timer_.async_wait(std::bind(&timer::on_timeout, this, _1));
  }
}

void timer::stop()
{
  error_code ec;
  timer_.cancel(ec);
}

void timer::on_timeout(const error_code& ec)
{
  if (!ec) {
    handler_();
  }

  if (repeat_ && ec != error::operation_aborted) {
    start();
  }
}

} // demo
} // asps
