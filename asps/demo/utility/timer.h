// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo timer.

#ifndef ASPS_DEMO_UTILITY_TIMER_H
#define ASPS_DEMO_UTILITY_TIMER_H

#include <asps/demo/session/timer_service.h>

namespace asps {
namespace demo {

class timer : public timer_service
{
public:
  void start(const std::chrono::seconds& seconds, callback_type callback) override
  {
    callback();
  }

  void stop() override
  {
  }
};

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_TIMER_H
