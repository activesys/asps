// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Session Service.

#ifndef ASPS_MODBUS_ADU_SEMANTIC_SESSION_SERVICE_HPP
#define ASPS_MODBUS_ADU_SEMANTIC_SESSION_SERVICE_HPP

#include <cstdint>
#include <memory>
#include <list>
#include <asps/utility/utility.h>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::utility;

// Client Session Service
class client_observer
{
public:
  virtual ~client_observer() {}

public:
  virtual void update_send(const buffer_type& adu) = 0;
  virtual void update_pdu(const buffer_type& pdu) = 0;
};

/*
 * Session service classe are also a subject
 */
class client_session_service
{
public:
  typedef std::shared_ptr<client_session_service> pointer_type;

public:
  virtual ~client_session_service() {}

public:
  virtual void receive_response(const buffer_type& adu) = 0;
  virtual void send_request(const buffer_type& pdu) = 0;

public:
  virtual void register_observer(client_observer* o) {os_.push_back(o);}
  virtual void notify_send(const buffer_type& buffer)
  {
    for (auto o : os_) {
      o->update_send(buffer);
    }
  }
  virtual void notify_pdu(const buffer_type& pdu)
  {
    for (auto o : os_) {
      o->update_pdu(pdu);
    }
  }

protected:
  std::list<client_observer*> os_;
};

client_session_service::pointer_type
make_client_session();

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEMANTIC_SESSION_SERVICE_HPP
