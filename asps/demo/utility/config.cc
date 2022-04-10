// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo config.

#include <asps/demo/utility/config.h>

namespace asps {
namespace demo {

int32_t config::t1_ = config::default_t1;
int32_t config::t2_ = config::default_t2;
uint8_t config::positive_keepalive_ack_ = config::default_positive_keepalive_ack;
bool config::same_type_ = config::default_same_type;
bool config::key_sequence_ = config::default_key_sequence;
bool config::same_timestamp_ = config::default_same_timestamp;

int32_t config::t1()
{
  return config::t1_;
}
void config::t1(int32_t t)
{
  config::t1_ = t;
}

int32_t config::t2()
{
  return config::t2_;
}
void config::t2(int32_t t)
{
  config::t2_ = t;
}

uint8_t config::positive_keepalive_ack()
{
  return config::positive_keepalive_ack_;
}
void config::positive_keepalive_ack(uint8_t ack)
{
  config::positive_keepalive_ack_ = ack;
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
