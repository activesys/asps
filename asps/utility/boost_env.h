// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Boost environment.

#ifndef ASPS_UTILITY_BOOST_ENV_H
#define ASPS_UTILITY_BOOST_ENV_H

#include <memory>
#include <boost/asio.hpp>

namespace asps {
namespace utility {

using namespace boost::asio;

extern std::shared_ptr<io_context> g_context;

} // utility
} // asps

#endif // ASPS_UTILITY_BOOST_ENV_H
