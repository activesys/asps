// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo config.

#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

uint32_t config::t1_ = config::default_t1;
uint32_t config::t2_ = config::default_t2;
uint8_t config::pack_ = config::default_pack;
bool config::same_type_ = config::default_same_type;
bool config::key_sequence_ = config::default_key_sequence;
bool config::same_timestamp_ = config::default_same_timestamp;

uint32_t config::t1()
{
  return config::t1_;
}

uint32_t config::t2()
{
  return config::t2_;
}

bool config::t1_t2(uint32_t t1, uint32_t t2)
{
  if (t1 > t2) {
    t1_ = t1;
    t2_ = t2;
    return true;
  } else {
    return false;
  }
}

uint8_t config::pack()
{
  return config::pack_;
}

void config::pack(uint8_t ack)
{
  config::pack_ = ack;
}

bool config::same_type()
{
  return config::same_type_;
}
void config::same_type(bool b)
{
  config::same_type_ = b;
}

bool config::key_sequence()
{
  return config::key_sequence_;
}
void config::key_sequence(bool b)
{
  config::key_sequence_ = b;
}

bool config::same_timestamp()
{
  return config::same_timestamp_;
}
void config::same_timestamp(bool b)
{
  config::same_timestamp_ = b;
}

} // demo
} // asps
