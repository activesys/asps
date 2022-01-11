// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus TCP ADU sequence.

#ifndef BOOST_MODBUS_ADU_TCPADUSEQ_H
#define BOOST_MODBUS_ADU_TCPADUSEQ_H

#include <cstdint>
#include <memory>
#include <asps/modbus/semantic/access_model.h>
#include <asps/modbus/adu/tcp_adu.h>

namespace asps {
namespace modbus {

// Modbus TCP ADU Sequence
class tcp_adu_state;
class tcp_adu_seq
{
public:
  tcp_adu_seq(uint16_t transaction_identifier)
    : transaction_identifier_(transaction_identifier)
  {}

public:
  const tcp_adu& send_request();
  void receive_response();

private:
  uint16_t transaction_identifier_;
  std::shared_ptr<tcp_adu_state> state_;
};

// Modbus TCP ADU Sequence state
class tcp_adu_state
{
public:
};

} // namespace modbus
} // namespace asps

#endif // BOOST_MODBUS_ADU_TCPADUSEQ_H
