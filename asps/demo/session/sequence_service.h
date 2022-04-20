// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session Sequence service.

#ifndef ASPS_DEMO_SESSION_SEQUENCE_SERVICE_H
#define ASPS_DEMO_SESSION_SEQUENCE_SERVICE_H

#include <cstdint>
#include <memory>
#include <list>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

enum sequence_type
{
  belong_to_data_sequence,
  belong_to_positive_keepalive_sequence,
  belong_to_negative_keepalive_sequence,
  belong_to_invalid_sequence
};

sequence_type get_sequence_type(const buffer_type& buffer);

class sequence_observer
{
public:
  virtual ~sequence_observer() {}

public:
  virtual void update_event() = 0;
};

class sequence_service
{
public:
  typedef std::shared_ptr<sequence_service> pointer_type;

public:
  sequence_service() = default;
  virtual ~sequence_service() {}

public:
  virtual const buffer_type& request() = 0;
  virtual bool response(buffer_type& buffer) = 0;

public:
  virtual void register_observer(sequence_observer* o) {sos_.push_back(o);}
  virtual void notify_event()
  {
    for (auto o : sos_) {
      o->update_event();
    }
  }

protected:
  std::list<sequence_observer*> sos_;
};

sequence_service::pointer_type
make_sequence_service(const data_group_type& type);
sequence_service::pointer_type
make_sequence_service(bool positive);
sequence_service::pointer_type
make_invalid_sequence();

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_SEQUENCE_SERVICE_H
