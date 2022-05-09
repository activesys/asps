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

sequence_type get_client_sequence_type(const buffer_type& buffer);
sequence_type get_server_sequence_type(const buffer_type& buffer);

// Data Send Sequence
class data_send_sequence_service
{
public:
  typedef std::shared_ptr<data_send_sequence_service> pointer_type;

public:
  virtual ~data_send_sequence_service() {}

public:
  virtual const buffer_type& send_data(const data_group_type& group) = 0;
};

data_send_sequence_service::pointer_type
make_data_send_sequence();

// Data Receive Sequence
class data_observer
{
public:
  ~data_observer() {}

public:
  virtual void update_data(const data_group_type& datas) = 0;
};

class data_receive_sequence_service
{
public:
  typedef std::shared_ptr<data_receive_sequence_service> pointer_type;

public:
  virtual ~data_receive_sequence_service() {}

public:
  virtual bool receive_data(buffer_type& buffer) = 0;

public:
  virtual void register_data_observer(data_observer* o) {dos_.push_back(o);}
  virtual void notify_data(const data_group_type& datas)
  {
    for (auto o : dos_) {
      o->update_data(datas);
    }
  }

protected:
  std::list<data_observer*> dos_;
};

data_receive_sequence_service::pointer_type
make_data_receive_sequence();

// Passive Sequence: receive request and send response
class passive_sequence_service
{
public:
  typedef std::shared_ptr<passive_sequence_service> pointer_type;

public:
  virtual ~passive_sequence_service() {}

public:
  virtual const buffer_type& send_response() = 0;
  virtual bool receive_request(buffer_type& buffer) = 0;
};

passive_sequence_service::pointer_type
make_passive_sequence(bool is_server);

// Active Sequence : send request and receive response
class event_observer
{
public:
  virtual ~event_observer() {}

public:
  virtual void update_event() = 0;
};

class active_sequence_service
{
public:
  typedef std::shared_ptr<active_sequence_service> pointer_type;

public:
  virtual ~active_sequence_service() {}

public:
  virtual const buffer_type& send_request() = 0;
  virtual bool receive_response(buffer_type& buffer) = 0;

public:
  virtual void register_event_observer(event_observer* o) {eos_.push_back(o);}
  virtual void notify_event()
  {
    for (auto o : eos_) {
      o->update_event();
    }
  }

protected:
  std::list<event_observer*> eos_;
};

active_sequence_service::pointer_type
make_active_sequence(bool is_server);

// Garbage Collector Sequence
// Receive and clear invalid byte
class garbage_collector_sequence_service
{
public:
  typedef std::shared_ptr<garbage_collector_sequence_service> pointer_type;

public:
  virtual ~garbage_collector_sequence_service() {}

public:
  virtual bool clear(buffer_type& buffer) = 0;
};

garbage_collector_sequence_service::pointer_type
make_garbage_collector_sequence();

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_SEQUENCE_SERVICE_H
