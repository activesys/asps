// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Message service.

#ifndef ASPS_MODBUS_ADU_SEQUENCE_MESSAGE_SERVICE_HPP
#define ASPS_MODBUS_ADU_SEQUENCE_MESSAGE_SERVICE_HPP

#include <memory>
#include <asps/utility/utility.h>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::utility;

class message_serialization_service
{
public:
  typedef std::shared_ptr<message_serialization_service> pointer_type;

public:
  virtual ~message_serialization_service() {}

public:
  virtual const buffer_type& serialize() = 0;
};

class message_unserialization_service
{
public:
  typedef std::shared_ptr<message_unserialization_service> pointer_type;

public:
  virtual ~message_unserialization_service() {}

public:
  virtual bool unserialize(const buffer_type& buffer) = 0;

public:
  const buffer_type& pdu()
  {return pdu_;}
  uint16_t tid()
  {return tid_;}
  uint16_t pid()
  {return pid_;}
  uint8_t uid()
  {return uid_;}

protected:
  buffer_type pdu_;
  uint16_t tid_;
  uint16_t pid_;
  uint8_t uid_;
};

message_serialization_service::pointer_type
make_client_request_adu(uint16_t tid, uint8_t uid, const buffer_type& pdu);
message_unserialization_service::pointer_type
make_client_response_adu();

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEQUENCE_MESSAGE_SERVICE_HPP
