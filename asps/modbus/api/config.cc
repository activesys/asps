// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Config.

#include <asps/modbus/api/config.h>

using namespace asps::modbus;

// Initialize config
uint16_t config::quantity_of_coils_ =
  config::max_quantity_of_coils;
uint16_t config::quantity_of_discrete_inputs_ =
  config::max_quantity_of_discrete_inputs;
uint16_t config::quantity_of_holding_registers_ =
  config::max_quantity_of_holding_registers;
uint16_t config::qunatity_of_input_registers_ =
  config::max_quantity_of_input_registers;
uint16_t config::quantity_of_write_coils_ =
  config::max_quantity_of_write_coils;
uint16_t config::quantity_of_write_registers_ =
  config::max_quantity_of_write_registers;
uint16_t config::quantity_of_concurrent_requests_ = 1;
