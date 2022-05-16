// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus command

#ifndef DEMOS_MODBUS_MODBUS_COMMAND_H
#define DEMOS_MODBUS_MODBUS_COMMAND_H

#include <asps/modbus/modbus.h>

namespace asps_demos {
namespace modbus_demos {

using namespace asps::modbus;

class mb_cmd
{
public:
  virtual ~mb_cmd() {}

public:
  virtual void do(const client& c) = 0;
};

class mb_read_coils : public mb_cmd
{
public:
  mb_read_coils(uint16_t address, uint16_t quantity)
    : cs_(address, quantity, memory_ + address)
  {}

public:
  virtual void do(const client& c) override
  {

  }

private:
  coils cs_;
  static bool memory_[0xffff];
}

} // modbus_demos
} // asps_demos

#endif // DEMOS_MODBUS_MODBUS_COMMAND_H
