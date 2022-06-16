// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Message constrant.

#ifndef ASPS_MODBUS_PDU_MESSAGE_MESSAGE_HPP
#define ASPS_MODBUS_PDU_MESSAGE_MESSAGE_HPP

enum message_filed_length
{
  function_code_filed_length = 1,
  exception_code_field_length = 1,
  address_field_length = 2,
  quantity_field_length = 2,
  byte_count_field_length = 1
};

#endif // ASPS_MODBUS_PDU_MESSAGE_MESSAGE_HPP
