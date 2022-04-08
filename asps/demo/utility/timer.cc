// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo timer.

#include <asps/demo/utility/timer.h>

namespace asps {
namespace demo {

timer_service::pointer_type make_timer_service()
{
  return std::make_shared<timer>();
}

} // demo
} // asps
