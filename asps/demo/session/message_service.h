// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session Message service.

#ifndef ASPS_DEMO_SESSION_MESSAGE_SERVICE_H
#define ASPS_DEMO_SESSION_MESSAGE_SERVICE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

class message_serialization_service
{
public:
  typedef std::shared_ptr<message_serialization_service> pointer_type;
  typedef std::vector<uint8_t> buffer_type;

public:
  message_serialization_service() = default;
  virtual ~message_serialization_service() {}

public:
  virtual const buffer_type& serialize() = 0;
};

message_serialization_service::pointer_type
make_message_serialization_service(const data_group_type& group);
message_serialization_service::pointer_type
make_message_serialization_service();

class message_unserialization_service
{
public:
  typedef std::shared_ptr<message_unserialization_service> pointer_type;

public:
  virtual ~message_unserialization_service() {}

public:
  virtual bool unserialize(const uint8_t* buffer) = 0;
};

message_unserialization_service::pointer_type
make_message_unserialization_service(uint8_t flag);

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_MESSAGE_SERVICE_H
