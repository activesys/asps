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

// Client Session Service
class client_observer
{
public:
  virtual ~client_observer() {}

public:
  virtual void update_send(const buffer_type& buffer) = 0;
  virtual void update_event() = 0;
};

/*
 * Session service classe are also a subject
 */
class client_session_service
{
public:
  typedef std::shared_ptr<client_session_service> pointer_type;

public:
  client_session_service() = default;
  virtual ~client_session_service() {}

public:
  virtual void receive(buffer_type& buffer) = 0;
  virtual bool send(const data_group_type& group) = 0;

public:
  virtual void register_observer(client_observer* o) {os_.push_back(o);}
  virtual void notify_send(const buffer_type& buffer)
  {
    for (auto o : os_) {
      o->update_send(buffer);
    }
  }
  virtual void notify_event()
  {
    for (auto o : os_) {
      o->update_event();
    }
  }

protected:
  std::list<client_observer*> os_;
};

client_session_service::pointer_type
make_client_session_service();

// Server Session Service
class server_observer
{
public:
  virtual ~server_observer() {}

public:
  virtual void update_data(const data_group_type& datas) = 0;
};

/*
 * Session service classe are also a subject
 */
class server_session_service
{
public:
  typedef std::shared_ptr<server_session_service> pointer_type;

public:
  server_session_service() = default;
  virtual ~server_session_service() {}

public:
  virtual void receive(buffer_type& buffer) = 0;

public:
  virtual void register_observer(server_observer* o) {os_.push_back(o);}
  virtual void notify_data(const data_group_type& datas)
  {
    for (auto o : os_) {
      o->update_data(datas);
    }
  }

protected:
  std::list<server_observer*> os_;
};

server_session_service::pointer_type
make_server_session_service();

} // demo
} // asps

#endif // ASPS_DEMO_API_SESSION_SERVICE_H
