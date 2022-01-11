// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session.

#include <deque>
#include <asps/modbus/config.h>
#include <asps/modbus/session/session.h>

using namespace asps::modbus;

// Modbus client session
void client_session::read_coils(const coils& cs)
{
  std::deque<coils::ptr_type> cs_queue;
  coils::split(cs, config::quantity_of_coils(), cs_queue);
}
