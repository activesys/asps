// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session Timer service.

#ifndef ASPS_DEMO_SESSION_TIMER_SERVICE_H
#define ASPS_DEMO_SESSION_TIMER_SERVICE_H

#include <memory>
#include <chrono>
#include <functional>

namespace asps {
namespace demo {

class timer_service
{
public:
  typedef std::function<void()> callback_type;
  typedef std::shared_ptr<timer_service> pointer_type;

public:
  timer_service() = default;
  virtual ~timer_service() {}

public:
  virtual void start(const std::chrono::seconds& seconds, callback_type callback) = 0;
  virtual void stop() = 0;
};

timer_service::pointer_type make_timer_service();

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_TIMER_SERVICE_H
