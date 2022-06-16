// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_ADU_SESSION_SESSION_HPP
#define ASPS_MODBUS_ADU_SESSION_SESSION_HPP

#include <unordered_map>
#include <asps/modbus/adu/semantic/session_service.hpp>
#include <asps/modbus/adu/session/sequence_service.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Modbus client session
class client_session
  : public client_session_service,
    public active_observer
{
public:
  virtual void receive_response(const buffer_type& adu) override;
  virtual void send_request(const buffer_type& pdu) override;

public:
  virtual void update_event() override;
  virtual void update_pdu(const buffer_type& pdu) override;

public:
  uint16_t get_tid(const buffer_type& adu);

private:
  std::unordered_map<uint16_t, active_sequence_service::pointer_type> seqs_;
  static uint16_t tid_;
};

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SESSION_SESSION_HPP
