// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#include <memory>
#include <asps/demo/session/demo_session.h>

namespace asps {
namespace demo {

session_service::pointer_type make_session_service()
{
  return std::make_shared<demo_session>(false, false, false);
}

} // demo
} // asps

