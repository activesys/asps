// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo config.

#ifndef ASPS_DEMO_CONFIG_CONFIG_H
#define ASPS_DEMO_CONFIG_CONFIG_H

#include <cstdint>
#include <string>

namespace asps {
namespace demo {

class config
{
  enum default_value {
    default_t0 = 30,
    default_t1 = 30,
    default_t2 = 20,
    default_pack = 0xff,
    default_nkeep = 0x00,
    default_same_type = true,
    default_key_sequence = true,
    default_same_timestamp = true,
    default_read_buffer_size = 65536
  };

public:
  static uint32_t t0();
  static void t0(uint32_t t);
  static uint32_t t1();
  static uint32_t t2();
  static bool t1_t2(uint32_t t1, uint32_t t2);
  static uint8_t pack();
  static uint8_t nkeep();
  static bool pack_nkeep(uint8_t pack, uint8_t nkeep);
  static bool same_type();
  static void same_type(bool b);
  static bool key_sequence();
  static void key_sequence(bool b);
  static bool same_timestamp();
  static void same_timestamp(bool b);
  static std::size_t read_buffer_size();
  static void read_buffer_size(std::size_t size);

private:
  static uint32_t t0_;
  static uint32_t t1_;
  static uint32_t t2_;
  static uint8_t pack_;
  static uint8_t nkeep_;
  static bool same_type_;
  static bool key_sequence_;
  static bool same_timestamp_;
  static std::size_t read_buffer_size_;
};

} // demo
} // asps

#endif // ASPS_DEMO_CONFIG_CONFIG_H
