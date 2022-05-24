// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Timer service.

#ifndef ASPS_UTILITY_TIMER_SERVICE_H
#define ASPS_UTILITY_TIMER_SERVICE_H

#include <memory>
#include <functional>

namespace asps {
namespace utility {

class timer_service
{
public:
  typedef std::function<void()> timeout_handler;
  typedef std::shared_ptr<timer_service> pointer_type;

public:
  timer_service(uint32_t expiry, timeout_handler handler, bool repeat)
    : expiry_(expiry),
      handler_(handler),
      repeat_(repeat)
  {}
  virtual ~timer_service() {}

public:
  virtual void start() = 0;
  virtual void stop() = 0;

protected:
  uint32_t expiry_;
  timeout_handler handler_;
  bool repeat_;
};

timer_service::pointer_type
make_timer(uint32_t expiry,
           timer_service::timeout_handler handler,
           bool repeat = false);

} // utility
} // asps

#endif // ASPS_UTILITY_TIMER_SERVICE_H
