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

// Connection
class connection
{
public:
  typedef std::shared_ptr<connection> pointer_type;

  typedef std::function<void(connection::pointer_type,
                             const buffer_type&,
                             std::size_t)> read_handler;
  typedef std::function<void(connection::pointer_type,
                             std::size_t)> write_handler;

public:
  connection() = default;
  virtual ~connection() {}

public:
  virtual void read(const read_handler& handler) = 0;
  virtual void write(const buffer_type& buf, const write_handler& handler) = 0;
  virtual void close() = 0;
};

connection::pointer_type
make_connection();

// Connector
class connector
{
public:
  typedef std::shared_ptr<connector> pointer_type;

  typedef std::function<void(bool,connection::pointer_type)> connect_handler;

public:
  connector(const std::string& ip, uint16_t port)
    : ip_(ip),
      port_(port)
  {}
  virtual ~connector() {}

public:
  virtual void connect(const connect_handler& handler) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;

protected:
  std::string ip_;
  uint16_t port_;
};

connector::pointer_type
make_connector(const std::string& ip, uint16_t port);

// Acceptor
class acceptor
{
public:
  typedef std::shared_ptr<acceptor> pointer_type;

  typedef std::function<void(connection::pointer_type)> accept_handler;

public:
  acceptor(const std::string& ip, uint16_t port)
    : ip_(ip),
      port_(port)
  {}
  acceptor(uint16_t port)
    : port_(port)
  {}
  virtual ~acceptor() {}

public:
  virtual void accept(const accept_handler& handler) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;

protected:
  std::string ip_;
  uint16_t port_;
};

acceptor::pointer_type
make_acceptor(const std::string& ip, uint16_t port);
acceptor::pointer_type
make_acceptor(uint16_t port);

} // demo
} // asps

#endif // ASPS_DEMO_API_TRANSPORT_SERVICE_H
