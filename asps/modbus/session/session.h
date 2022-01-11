// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#ifndef ASPS_MODBUS_SESSION_SESSION_H
#define ASPS_MODBUS_SESSION_SESSION_H

#include <cstdint>
#include <boost/core/noncopyable.hpp>

#include <asps/modbus/semantic/coils.h>

namespace asps {
namespace modbus {

// Modbus client session
class client_session : private boost::noncopyable
{
public:
  client_session()
    : transaction_identifier_(0)
  {}

public:
  void read_coils(const coils& cs);

private:

private:
  uint16_t transaction_identifier_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SESSION_SESSION_H
