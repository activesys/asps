// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Data.

#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

bool operator==(const demo_data::pointer_type first,
                const demo_data::pointer_type second)
{
  return *first == *second;
}

} // demo
} // asps
