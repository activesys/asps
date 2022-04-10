// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo config.

#ifndef ASPS_DEMO_UTILITY_CONFIG_H
#define ASPS_DEMO_UTILITY_CONFIG_H

#include <cstdint>

namespace asps {
namespace demo {

class config
{
  enum default_value {
    default_t1 = 30,
    default_t2 = 20,
    default_positive_keepalive_ack = 0xff,
    default_same_type = false,
    default_key_sequence = false,
    default_same_timestamp = false
  };

public:
  static int32_t t1();
  static void t1(int32_t t);
  static int32_t t2();
  static void t2(int32_t t);
  static uint8_t positive_keepalive_ack();
  static void positive_keepalive_ack(uint8_t ack);
  static bool same_type();
  static void same_type(bool b);
  static bool key_sequence();
  static void key_sequence(bool b);
  static bool same_timestamp();
  static void same_timestamp(bool b);

private:
  static int32_t t1_;
  static int32_t t2_;
  static uint8_t positive_keepalive_ack_;
  static bool same_type_;
  static bool key_sequence_;
  static bool same_timestamp_;
};

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_CONFIG_H
