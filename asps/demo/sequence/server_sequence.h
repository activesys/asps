// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server Sequence.

#ifndef ASPS_DEMO_SEQUENCE_SERVER_SEQUENCE_H
#define ASPS_DEMO_SEQUENCE_SERVER_SEQUENCE_H

#include <asps/demo/utility/utility.h>
#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/session/sequence_service.h>
#include <asps/demo/message/server_message.h>

namespace asps {
namespace demo {

// Server Data Sequence
class server_data_sequence
  : public server_data_sequence_service
{
public:
  server_data_sequence();

public:
  virtual bool response(buffer_type& buffer) override;

private:
  message_unserialization_service::pointer_type message_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_SERVER_SEQUENCE_H
