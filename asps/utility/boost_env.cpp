// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Boost environment.

#include <asps/utility/boost_env.hpp>

namespace asps {
namespace utility {

std::shared_ptr<io_context> g_context = std::make_shared<io_context>();

} // utility
} // asps
