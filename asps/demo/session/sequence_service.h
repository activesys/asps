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

// Client Data Sequence
class client_data_sequence_service
{
public:
  typedef std::shared_ptr<client_data_sequence_service> pointer_type;

public:
  virtual ~client_data_sequence_service() {}

public:
  virtual const buffer_type& request(const data_group_type& group) = 0;
};

client_data_sequence_service::pointer_type
make_client_data_sequence();

// Client Negative Keepalive Sequence
class client_negative_keepalive_sequence_service
{
public:
  typedef std::shared_ptr<client_negative_keepalive_sequence_service> pointer_type;

public:
  virtual ~client_negative_keepalive_sequence_service() {}

public:
  virtual const buffer_type& request() = 0;
  virtual bool response(buffer_type& buffer) = 0;
};

client_negative_keepalive_sequence_service::pointer_type
make_client_negative_keepalive_sequence();

// Client Positive Keepalive Sequence
class event_observer
{
public:
  virtual ~event_observer() {}

public:
  virtual void update_event() = 0;
};

class client_positive_keepalive_sequence_service
{
public:
  typedef std::shared_ptr<client_positive_keepalive_sequence_service> pointer_type;

public:
  virtual ~client_positive_keepalive_sequence_service() {}

public:
  virtual const buffer_type& request() = 0;
  virtual bool response(buffer_type& buffer) = 0;

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

client_positive_keepalive_sequence_service::pointer_type
make_client_positive_keepalive_sequence();

// Client Invalid Sequence
class client_invalid_sequence_service
{
public:
  typedef std::shared_ptr<client_invalid_sequence_service> pointer_type;

public:
  virtual ~client_invalid_sequence_service() {}

public:
  virtual bool response(buffer_type& buffer) = 0;
};

client_invalid_sequence_service::pointer_type
make_client_invalid_sequence();

// Server Data Sequence
class data_observer
{
public:
  ~data_observer() {}

public:
  virtual void update_data(const data_group_type& datas) = 0;
};

class server_data_sequence_service
{
public:
  typedef std::shared_ptr<server_data_sequence_service> pointer_type;

public:
  virtual ~server_data_sequence_service() {}

public:
  virtual bool response(buffer_type& buffer) = 0;

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

server_data_sequence_service::pointer_type
make_server_data_sequence();

} // demo
} // asps

#endif // ASPS_DEMO_SESSION_SEQUENCE_SERVICE_H
