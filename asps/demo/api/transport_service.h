// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Transport service.

#ifndef ASPS_DEMO_API_TRANSPORT_SERVICE_H
#define ASPS_DEMO_API_TRANSPORT_SERVICE_H

#include <string>
#include <memory>
#include <functional>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

// Client Transport Service
class client_transport_service
{
public:
  typedef std::shared_ptr<client_transport_service> pointer_type;

  typedef std::function<void(bool)> connect_handler;
  typedef std::function<void(bool,std::size_t)> write_handler;
  typedef std::function<void(bool,const buffer_type&,std::size_t)> read_handler;

public:
  client_transport_service() = default;
  virtual ~client_transport_service() {}

public:
  virtual void read(const read_handler& handler) = 0;
  virtual void connect(const connect_handler& handler) = 0;
  virtual void close() = 0;
  virtual void write(const buffer_type& buf, const write_handler& handler) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;
};

client_transport_service::pointer_type
make_client_transport_service();

// Server Transport Service
class server_transport_service
{
public:
  typedef std::shared_ptr<server_transport_service> pointer_type;

  typedef std::function<void(bool)> accept_handler;
  typedef std::function<void(bool,const buffer_type&,std::size_t)> read_handler;

public:
  server_transport_service() = default;
  virtual ~server_transport_service() {}

public:
  virtual void accept(const accept_handler& handler) = 0;
  virtual void read(const read_handler& handler) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;
};

server_transport_service::pointer_type
make_server_transport_service(const std::string& ip, uint16_t port);
server_transport_service::pointer_type
make_server_transport_service(uint16_t port);

} // demo
} // asps

#endif // ASPS_DEMO_API_TRANSPORT_SERVICE_H
