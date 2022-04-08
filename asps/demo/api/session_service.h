// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo session service interface.

#ifndef ASPS_DEMO_API_SESSION_SERVICE_H
#define ASPS_DEMO_API_SESSION_SERVICE_H

#include <memory>
#include <vector>
#include <list>
#include <cstdint>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

class observer
{
public:
  virtual ~observer() {}

public:
  virtual void update_positive_keepalive() = 0;
  virtual void update_missing_positive_keepalive_ack() = 0;
};

/*
 * Session service classe are also a subject
 */
class session_service
{
public:
  typedef std::shared_ptr<session_service> pointer_type;
  typedef std::vector<uint8_t> buffer_type;

public:
  session_service() = default;
  virtual ~session_service() {}

public:
  virtual buffer_type& serialize_datas(const data_group_type& group) = 0;
  virtual buffer_type& serialize_keepalive() = 0;
  virtual bool receive(const uint8_t* buffer) = 0;
  virtual void same_type(bool flag) = 0;
  virtual void key_sequence(bool flag) = 0;
  virtual void same_timestamp(bool flag) = 0;

public:
  virtual void register_observer(observer* o) {os_.push_back(o);}
  virtual void notify_positive_keepalive()
  {
    for (auto o : os_) {
      o->update_positive_keepalive();
    }
  }
  virtual void notify_missing_positive_keepalive_ack()
  {
    for (auto o : os_) {
      o->update_missing_positive_keepalive_ack();
    }
  }

protected:
  std::list<observer*> os_;
};

session_service::pointer_type make_session_service();

} // demo
} // asps

#endif // ASPS_DEMO_API_SESSION_SERVICE_H
