// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo timer.

#ifndef ASPS_DEMO_UTILITY_TIMER_H
#define ASPS_DEMO_UTILITY_TIMER_H

#include <memory>
#include <boost/asio.hpp>
#include <asps/demo/utility/timer_service.h>

namespace asps {
namespace demo {

using namespace boost::asio;
using namespace boost::system;

extern std::shared_ptr<io_context> g_context;

class timer : public timer_service
{
public:
  timer(std::shared_ptr<io_context> context,
        uint32_t expiry,
        timer_service::timeout_handler handler)
    : timer_service(expiry, handler),
      context_(context),
      timer_(*context_)
  {}

public:
  void start() override;
  void stop() override;

private:
  void on_timeout(const error_code& ec);

private:
  std::shared_ptr<io_context> context_;
  steady_timer timer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_TIMER_H
