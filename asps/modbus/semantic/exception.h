// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Exception Codes.

#ifndef ASPS_MODBUS_SEMANTIC_EXCEPTION_H
#define ASPS_MODBUS_SEMANTIC_EXCEPTION_H

namespace asps {
namespace modbus {

enum exception_code {
  success = 0x00,
  illegal_function = 0x01,
  illegal_data_address = 0x02,
  illegal_data_value = 0x03,
  server_device_failure = 0x04,
  acknowledge = 0x05,
  service_device_busy = 0x06,
  memory_parity_error = 0x08,
  gateway_path_unavailable = 0x0a,
  gateway_target_device_failed_to_response = 0x0b
};

} // modbus
} // asps

#endif // ASPS_MODBUS_SEMANTIC_EXCEPTION_H