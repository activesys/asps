// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Data.

#ifndef ASPS_DEMO_SEMANTIC_DEMO_DATA_H
#define ASPS_DEMO_SEMANTIC_DEMO_DATA_H

#include <cstdint>

namespace asps {
namespace demo {

template <typename T>
struct demo_data
{
  uint32_t key;
  T value;
  uint64_t timestamp;
};

} // demo
} // asps

#endif // ASPS_DEMO_SEMANTIC_DEMO_DATA_H
