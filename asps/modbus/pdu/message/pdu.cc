// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU.

#include <netinet/in.h>
#include <asps/modbus/pdu/message/pdu.h>

using namespace asps::modbus;

// Modbus PDU
pdu_ptr pdu::unserialize(const uint8_t* buffer, bool is_request)
{
  // decode function code
  uint8_t function_code = *buffer;
  if (function_code & excep_pdu::exception_id) {
    return excep_pdu::unserialize(buffer);
  } else {
    switch (function_code) {
    case read_coils:
      if (is_request) {
        return read_coils_request::unserialize(buffer);
      } else {
        return read_coils_response::unserialize(buffer);
      }
      break;

    default:
      // We make an invalid pdu exception pdu
      return std::make_shared<excep_pdu>(invalid_pdu, server_device_failure);
      break;
    }
  }
}

// Modbus Exception PDU
std::size_t excep_pdu::serialized_size()
{
  return function_code_field_length +
         exception_code_field_length;
}

uint8_t* excep_pdu::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode function code
  *pos = function_code_;
  pos += function_code_field_length;
  // encode exception code;
  *pos = code_;
  pos += exception_code_field_length;

  return buffer_.data();
}

pdu_ptr excep_pdu::unserialize(const uint8_t* buffer)
{
  const uint8_t* pos = buffer;
  // decode function code
  uint8_t function_code = *pos & ~static_cast<uint8_t>(exception_id);
  pos += function_code_field_length;
  // decode exception code
  exception_code code = static_cast<exception_code>(*pos);
  pos += exception_code_field_length;

  return std::make_shared<excep_pdu>(function_code, code);
}

// Modbus Read Coils request PDU
std::size_t read_coils_request::serialized_size()
{
  return function_code_field_length +
         starting_address_field_length +
         quantity_of_coils_field_length;
}

uint8_t* read_coils_request::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode function code
  *pos = function_code_;
  pos += function_code_field_length;
  // encode starting address
  *reinterpret_cast<uint16_t*>(pos) = htons(starting_address_);
  pos += starting_address_field_length;
  // encode quantity of coils
  *reinterpret_cast<uint16_t*>(pos) = htons(quantity_of_coils_);
  pos += quantity_of_coils_field_length;

  return buffer_.data();
}

pdu_ptr read_coils_request::unserialize(const uint8_t* buffer)
{
  const uint8_t* pos = buffer;
  // decode function code
  uint8_t function_code = *pos;
  pos += function_code_field_length;
  // decode starting address
  uint16_t starting_address =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  pos += starting_address_field_length;
  // decode quantity of coils
  uint16_t quantity_of_coils =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  pos += quantity_of_coils_field_length;

  return std::make_shared<read_coils_request>(
           starting_address, quantity_of_coils);
}

// Modbus Read Coils Response PDU
std::size_t read_coils_response::serialized_size()
{
  return function_code_field_length +
         byte_count_field_length +
         byte_count_;
}

uint8_t* read_coils_response::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode function code
  *pos = function_code_;
  pos += function_code_field_length;
  // encode byte count
  *pos = byte_count_;
  pos += byte_count_field_length;
  // ecnode coils status
  for (uint8_t i = 0; i < coils_status_.size(); ++i) {
    uint8_t value = coils_status_.at(i);
    pos[i/8] |= value << i % 8;
  }
  pos += byte_count_;

  return buffer_.data();
}

pdu_ptr read_coils_response::unserialize(const uint8_t* buffer)
{
  const uint8_t* pos = buffer;
  // decode function code
  uint8_t function_code = *pos;
  pos += function_code_field_length;
  // decode byte count
  uint8_t byte_count = *pos;
  pos += byte_count_field_length;
  // decode coils status
  // We assume that all the bits in coils status byte are valid,
  // so we may have more data than we actually have.
  coils_type coils_status(byte_count * 8);
  for (uint8_t i = 0; i < byte_count * 8; ++i) {
    coils_status[i] = (pos[i/8] >> i%8) & 0x01;
  }
  pos += byte_count;

  return std::make_shared<read_coils_response>(coils_status);
}

// Modbus Write Single Coil
std::size_t write_single_coil_request::serialized_size()
{
  return function_code_field_length +
         output_address_field_length +
         output_value_field_length;
}

uint8_t* write_single_coil_request::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode function code
  *pos = function_code_;
  pos += function_code_field_length;
  // encode output address
  *reinterpret_cast<uint16_t*>(pos) = htons(output_address_);
  pos += output_address_field_length;
  // encode output value
  if (output_value_) {
    *reinterpret_cast<uint16_t*>(pos) = htons(output_value_on);
  } else {
    *reinterpret_cast<uint16_t*>(pos) = htons(output_value_off);
  }
  pos += output_value_field_length;

  return buffer_.data();
}

pdu_ptr write_single_coil_request::unserialize(const uint8_t* buffer)
{
  const uint8_t* pos = buffer;
  // decode function code
  uint8_t function_code = *pos;
  pos += function_code_field_length;
  // decode output address
  uint16_t output_address =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  pos += output_address_field_length;
  // decode output value
  bool output_value =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos))) ==
    output_value_on;
  pos += output_value_field_length;

  return std::make_shared<write_single_coil_request>(
           output_address, output_value);
}

// Modbus Write Multiple Coils
std::size_t write_multiple_coils_request::serialized_size()
{
  return function_code_field_length +
         starting_address_field_length +
         quantity_of_outputs_field_length +
         byte_count_field_length +
         byte_count_;
}

uint8_t* write_multiple_coils_request::serialize()
{
  buffer_.resize(serialized_size());
  uint8_t* pos = buffer_.data();
  // encode function code
  *pos = function_code_;
  pos += function_code_field_length;
  // encode starting address
  *reinterpret_cast<uint16_t*>(pos) = htons(starting_address_);
  pos += starting_address_field_length;
  // encode quantity of outputs
  *reinterpret_cast<uint16_t*>(pos) = htons(quantity_of_outputs_);
  pos += quantity_of_outputs_field_length;
  // encode byte count
  *pos = byte_count_;
  pos += byte_count_field_length;
  // encode output values
  for (uint8_t i = 0; i < quantity_of_outputs_; ++i) {
    uint8_t value = outputs_value_[i];
    pos[i/8] != value << i % 8;
  }
  pos += byte_count_;

  return buffer_.data();
}

pdu_ptr write_multiple_coils_request::unserialize(const uint8_t* buffer)
{
  const uint8_t* pos = buffer;
  // decode function code
  uint8_t function_code = *pos;
  pos += function_code_field_length;
  // decode starting address
  uint16_t starting_address =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  pos += starting_address_field_length;
  // decode quantity of outputs
  uint16_t quantity_of_outputs =
    ntohs(*reinterpret_cast<uint16_t*>(const_cast<uint8_t*>(pos)));
  pos += quantity_of_outputs_field_length;
  // decode byte count
  uint8_t byte_count = *pos;
  pos += byte_count_field_length;
  // decode outputs value
  // We assume that all the bits in coils status byte are valid,
  // so we may have more data than we actually have.
  coils_type outputs_value(byte_count * 8);
  for (uint8_t i = 0; i < byte_count * 8; ++i) {
    outputs_value[i] = (pos[i/8] >> i%8) & 0x01;
  }
  pos += byte_count;

  return std::make_shared<write_multiple_coils_request>(
           starting_address, quantity_of_outputs, outputs_value);
}
