// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo utility.

#include <asps/demo/utility/utility.h>

namespace asps {
namespace demo {

uint64_t htonll(uint64_t value)
{
  /*
   * Converts data to big-end when the host sequence is little-end.
   * The method of conversion is to swap 8 bytes with each other.
   */
#if (ASPS_BIG_ENDIAN)
  return value;
#else
  char c = 0;
  int  i = 0;
  union {
    uint64_t    v;
    char        c[8];
  } x;

  x.v = value;
  for (i = 0; i < 4; ++i) {
    c = x.c[i];
    x.c[i] = x.c[7-i];
    x.c[7-i] = c;
  }

  return x.v;
#endif
}

uint64_t ntohll(uint64_t value)
{
  return htonll(value);
}

} // demo
} // asps
