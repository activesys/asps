// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Message constrant.

#ifndef ASPS_MODBUS_ADU_MESSAGE_MESSAGE_HPP
#define ASPS_MODBUS_ADU_MESSAGE_MESSAGE_HPP

enum message_field_length
{
  transaction_identifier_field_length = 2,
  protocol_identifier_field_length = 2,
  length_field_length = 2,
  unit_identifier_field_length = 1,
  mbap_field_length = 7
};

#endif // ASPS_MODBUS_ADU_MESSAGE_MESSAGE_HPP
