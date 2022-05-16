// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Default Transport.

#ifndef ASPS_DEMO_UTILITY_TRANSPORT_H
#define ASPS_DEMO_UTILITY_TRANSPORT_H

#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <asps/demo/config/config.h>
#include <asps/demo/api/transport_service.h>

namespace asps {
namespace demo {

using namespace boost::asio;
using namespace boost::system;

extern std::shared_ptr<io_context> g_context;

// Connection
class boost_connection
  : public std::enable_shared_from_this<boost_connection>,
    public connection
{
public:
  boost_connection(std::shared_ptr<io_context> context)
    : context_(context),
      socket_(*context_)
  {
    buffer_.resize(config::read_buffer_size());
  }
  virtual ~boost_connection() {}

public:
  virtual void set_handler(const read_handler rhandler,
                           const write_handler whandler,
                           const close_handler chandler) override;
  virtual void read() override;
  virtual void write(const buffer_type& buf) override;
  virtual void close() override;
  virtual std::string remote_address() override;
  virtual uint16_t remote_port() override;

public:
  ip::tcp::socket& socket()
  {
    return socket_;
  }

private:
  void on_read(const error_code& ec, std::size_t bytes);
  void on_write(const error_code& ec, std::size_t bytes);

private:
  std::shared_ptr<io_context> context_;
  ip::tcp::socket socket_;
  buffer_type buffer_;
  read_handler read_handler_;
  write_handler write_handler_;
  close_handler close_handler_;
  std::string remote_ip_;
  uint16_t remote_port_;
};

// Connector
class boost_connector
  : public connector
{
public:
  boost_connector(std::shared_ptr<io_context> context,
                  const std::string& ip,
                  uint16_t port)
    : connector(ip, port),
      context_(context),
      connection_(make_connection())
  {}
  virtual ~boost_connector() {}

public:
  virtual void set_handler(const connect_handler handler) override;
  virtual void connect() override;
  virtual void run() override;
  virtual void stop() override;

private:
  void do_connect();
  void on_connect(const error_code& ec);

private:
  std::shared_ptr<io_context> context_;
  connect_handler connect_handler_;
  connection::pointer_type connection_;
};

// Acceptor
class boost_acceptor
  : public acceptor
{
public:
  boost_acceptor(std::shared_ptr<io_context> context,
                 const std::string& ip,
                 uint16_t port)
    : acceptor(ip, port),
      context_(context),
      acceptor_(*context_,
                ip::tcp::endpoint(ip::address::from_string(ip), port))
  {}
  boost_acceptor(std::shared_ptr<io_context> context,
                 uint16_t port)
    : acceptor(port),
      context_(context),
      acceptor_(*context_,
                ip::tcp::endpoint(ip::tcp::v4(), port))
  {}
  virtual ~boost_acceptor() {}

public:
  virtual void set_handler(const accept_handler ahandler,
                           const release_handler rhandler) override;
  virtual void accept() override;
  virtual void release(connection::pointer_type conn) override;
  virtual void run() override;
  virtual void stop() override;

private:
  void do_accept();
  void on_accept(const error_code& ec);
  void on_release(connection::pointer_type conn);

private:
  std::shared_ptr<io_context> context_;
  ip::tcp::acceptor acceptor_;
  connection::pointer_type connection_;
  accept_handler accept_handler_;
  release_handler release_handler_;
};

} // demo
} // asps

#endif // ASPS_DEMO_UTILITY_TRANSPORT_H
