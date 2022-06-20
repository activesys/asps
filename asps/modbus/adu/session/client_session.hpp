// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Client Session.

#ifndef ASPS_MODBUS_ADU_SESSION_CLIENT_SESSION_HPP
#define ASPS_MODBUS_ADU_SESSION_CLIENT_SESSION_HPP

#include <unordered_map>
#include <asps/modbus/adu/sequence/client_sequence.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Client Session Observer
class client_session_observer
{
public:
  virtual ~client_session_observer() {}

public:
  virtual void update_send(const buffer_type& adu) = 0;
  virtual void update_pdu(const buffer_type& pdu) = 0;
};

// Modbus client session
class client_session
  : public client_sequence_observer
{
public:
  void receive_response(const buffer_type& adu);
  void send_request(const buffer_type& pdu);

public:
  virtual void update_pdu(const buffer_type& pdu) override;

public:
  virtual void register_observer(client_session_observer* obj)
  {csobjs_.push_back(obj);}
  virtual void notify_send(const buffer_type& buffer)
  {
    for (auto obj : csobjs_) {
      obj->update_send(buffer);
    }
  }
  virtual void notify_pdu(const buffer_type& pdu)
  {
    for (auto obj : csobjs_) {
      obj->update_pdu(pdu);
    }
  }

public:
  uint16_t get_tid(const buffer_type& adu);

private:
  std::list<client_session_observer*> csobjs_;
  std::unordered_map<uint16_t, client_sequence::pointer_type> seqs_;
  static uint16_t tid_;
};

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SESSION_CLIENT_SESSION_HPP
