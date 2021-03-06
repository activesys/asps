// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client Sequence.

#ifndef ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H
#define ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H

#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/sequence/sequence.h>

namespace asps {
namespace demo {

// Client Data Sequence
class client_data_sequence
  : public data_send_sequence_service
{
public:
  const buffer_type& send_data(const data_group_type& group) override;

private:
  message_serialization_service::pointer_type message_;
};

// Client Positive Keepalive Sequence
class client_positive_keepalive_sequence
  : public active_sequence
{
public:
  client_positive_keepalive_sequence();
  virtual ~client_positive_keepalive_sequence() {}
};

// Client Negative Keepalive Sequence
class client_negative_keepalive_sequence
  : public passive_sequence
{
public:
  client_negative_keepalive_sequence();
  virtual ~client_negative_keepalive_sequence() {}
};

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_CLIENT_SEQUENCE_H
