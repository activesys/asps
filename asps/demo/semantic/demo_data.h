// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Data.

#ifndef ASPS_DEMO_SEMANTIC_DEMO_DATA_H
#define ASPS_DEMO_SEMANTIC_DEMO_DATA_H

#include <cstdint>
#include <arpa/inet.h>

namespace asps {
namespace demo {

enum value_type {
  invalid_type = -1,
  boolean_type = 0,
  int8_type = 1,
  uint8_type = 2,
  int16_type = 3,
  uint16_type = 4,
  int32_type = 5,
  uint32_type = 6,
  int64_type = 7,
  uint64_type = 8,
  float32_type = 10,
  float64_type = 11
};

// host sequence to network sequence for 64bits integer.
uint64_t htonll(uint64_t value);

// Demo Data
template <typename T>
struct demo_data
{
  demo_data(uint32_t k, T v, uint64_t t)
    : type(invalid_type),
      key(k),
      value(v),
      timestamp(t)
  {}

  std::size_t size() {return sizeof(T);}
  T network_sequence_of_value() {return value;}

  value_type type;
  uint32_t key;
  T value;
  uint64_t timestamp;
};

#define MAKE_DEMO_DATA_SPECILICATION(TT, VT, FT)              \
  template <>                                                 \
  struct demo_data<TT>                                        \
  {                                                           \
    demo_data(uint32_t k, TT v, uint64_t t)                   \
      : type(VT),                                             \
        key(k),                                               \
        value(v),                                             \
        timestamp(t)                                          \
    {}                                                        \
                                                              \
    std::size_t size() {return sizeof(TT);}                   \
    TT network_sequence_of_value() {return FT(value);}        \
                                                              \
    value_type type;                                          \
    uint32_t key;                                             \
    TT value;                                                 \
    uint64_t timestamp;                                       \
  };

MAKE_DEMO_DATA_SPECILICATION(bool, boolean_type, bool)
MAKE_DEMO_DATA_SPECILICATION(int8_t, int8_type, int8_t)
MAKE_DEMO_DATA_SPECILICATION(uint8_t, uint8_type, uint8_t)
MAKE_DEMO_DATA_SPECILICATION(int16_t, int16_type, htons)
MAKE_DEMO_DATA_SPECILICATION(uint16_t, uint16_type, htons)
MAKE_DEMO_DATA_SPECILICATION(int32_t, int32_type, htonl)
MAKE_DEMO_DATA_SPECILICATION(uint32_t, uint32_type, htonl)
MAKE_DEMO_DATA_SPECILICATION(int64_t, int64_type, htonll)
MAKE_DEMO_DATA_SPECILICATION(uint64_t, uint64_type, htonll)
MAKE_DEMO_DATA_SPECILICATION(float, float32_type, float)
MAKE_DEMO_DATA_SPECILICATION(double, float64_type, double)

#undef MAKE_DEMO_DATA_SPECILICATION


} // demo
} // asps

#endif // ASPS_DEMO_SEMANTIC_DEMO_DATA_H
