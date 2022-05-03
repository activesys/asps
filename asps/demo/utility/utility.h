// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo utility.

#ifndef ASPS_DEMO_UTILITY_UTILITY_H
#define ASPS_DEMO_UTILITY_UTILITY_H

#include <cstdint>
#include <vector>

namespace asps {
namespace demo {

typedef std::vector<uint8_t> buffer_type;

// host sequence to network sequence for 64bits integer.
uint64_t htonll(uint64_t value);
uint64_t ntohll(uint64_t value);

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_UTILITY_H
