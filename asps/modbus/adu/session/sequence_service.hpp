// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Active Sequence Interface.

#ifndef ASPS_MODBUS_ADU_SESSION_SEQUENCE_SERVICE_HPP
#define ASPS_MODBUS_ADU_SESSION_SEQUENCE_SERVICE_HPP

#include <memory>
#include <list>
#include <asps/utility/utility.h>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::utility;

// Active Sequence : send request and receive response
class active_observer
{
public:
  virtual ~active_observer() {}

public:
  virtual void update_event() = 0;
  virtual void update_pdu(const buffer_type& pdu) = 0;
};

class active_sequence_service
{
public:
  typedef std::shared_ptr<active_sequence_service> pointer_type;

public:
  virtual ~active_sequence_service() {}

public:
  virtual const buffer_type& send_request() = 0;
  virtual bool receive_response(const buffer_type& adu) = 0;

public:
  virtual void register_event_observer(active_observer* o) {aos_.push_back(o);}
  virtual void notify_event()
  {
    for (auto o : aos_) {
      o->update_event();
    }
  }
  virtual void notify_datas(const buffer_type& pdu)
  {
    for (auto o : aos_) {
      o->update_pdu(pdu);
    }
  }

protected:
  std::list<active_observer*> aos_;
};

active_sequence_service::pointer_type
make_active_sequence(uint16_t tid, uint8_t uid, const buffer_type& pdu);

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SESSION_SEQUENCE_SERVICE_HPP
