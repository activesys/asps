// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Timer service.

#ifndef ASPS_DEMO_UTILITY_TIMER_SERVICE_H
#define ASPS_DEMO_UTILITY_TIMER_SERVICE_H

#include <memory>
#include <chrono>
#include <functional>

namespace asps {
namespace demo {

class timer_service
{
public:
  typedef std::function<void()> timeout_handler;
  typedef std::shared_ptr<timer_service> pointer_type;

public:
  timer_service(uint32_t expiry, timeout_handler handler)
    : expiry_(std::chrono::seconds(expiry)),
      handler_(handler)
  {}
  virtual ~timer_service() {}

public:
  virtual void start() = 0;
  virtual void stop() = 0;

protected:
  std::chrono::seconds expiry_;
  timeout_handler handler_;
};

timer_service::pointer_type
make_timer_service(uint32_t expiry,
                   timer_service::timeout_handler handler);

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_TIMER_SERVICE_H
