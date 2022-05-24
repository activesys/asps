// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Transport service.

#ifndef ASPS_TRANSPORT_TRANSPORT_SERVICE_H
#define ASPS_TRANSPORT_TRANSPORT_SERVICE_H

#include <string>
#include <memory>
#include <functional>
#include <asps/utility/utility.h>

namespace asps {
namespace transport {

using namespace asps::utility;

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
  typedef std::function<void(connection::pointer_type)> close_handler;

public:
  connection() = default;
  virtual ~connection() {}

public:
  virtual void set_handler(const read_handler rhandler,
                           const write_handler whandler,
                           const close_handler chandler) = 0;
  virtual void read() = 0;
  virtual void write(const buffer_type& buf) = 0;
  virtual void close() = 0;
  virtual std::string remote_address() = 0;
  virtual uint16_t remote_port() = 0;
};

connection::pointer_type
make_connection();

// Connector
class connector
{
public:
  typedef std::shared_ptr<connector> pointer_type;

  typedef std::function<void(connection::pointer_type)> connect_handler;

public:
  connector(const std::string& ip, uint16_t port)
    : ip_(ip),
      port_(port)
  {}
  virtual ~connector() {}

public:
  virtual void set_handler(const connect_handler handler) = 0;
  virtual void connect() = 0;
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
  typedef std::function<void(connection::pointer_type)> release_handler;

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
  virtual void set_handler(const accept_handler ahandler,
                           const release_handler rhandler) = 0;
  virtual void release(connection::pointer_type conn) = 0;
  virtual void accept() = 0;
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

} // transport
} // asps

#endif // ASPS_TRANSPORT_TRANSPORT_SERVICE_H
