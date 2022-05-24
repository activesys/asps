// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// timer.

#ifndef ASPS_UTILITY_TIMER_H
#define ASPS_UTILITY_TIMER_H

#include <memory>
#include <boost/asio.hpp>
#include <asps/utility/timer_service.h>

namespace asps {
namespace utility {

using namespace boost::asio;
using namespace boost::system;

class timer : public timer_service
{
public:
  timer(std::shared_ptr<io_context> context,
        uint32_t expiry,
        timer_service::timeout_handler handler,
        bool repeat)
    : timer_service(expiry, handler, repeat),
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

} // utility
} // asps

#endif // ASPS_UTILITY_TIMER_H
