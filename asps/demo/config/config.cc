// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo config.

#include <asps/demo/config/config.h>

namespace asps {
namespace demo {

uint32_t config::t0_ = config::default_t0;
uint32_t config::t1_ = config::default_t1;
uint32_t config::t2_ = config::default_t2;
uint8_t config::pack_ = config::default_pack;
uint8_t config::nkeep_ = config::default_nkeep;
bool config::same_type_ = config::default_same_type;
bool config::key_sequence_ = config::default_key_sequence;
bool config::same_timestamp_ = config::default_same_timestamp;
std::size_t config::read_buffer_size_ = config::default_read_buffer_size;

uint32_t config::t0()
{
  return config::t0_;
}

void config::t0(uint32_t t)
{
  config::t0_ = t;
}

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

uint8_t config::nkeep()
{
  return config::nkeep_;
}

bool config::pack_nkeep(uint8_t pack, uint8_t nkeep)
{
  if (pack != nkeep) {
    config::pack_ = pack;
    config::nkeep_ = nkeep;
    return true;
  } else {
    return false;
  }
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

std::size_t config::read_buffer_size()
{
  return config::read_buffer_size_;
}

void config::read_buffer_size(std::size_t size)
{
  config::read_buffer_size_ = size;
}

} // demo
} // asps
