// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Transport layer services implemented using Boost.

#ifndef ASPS_TRANSPORT_TRANSPORT_HPP
#define ASPS_TRANSPORT_TRANSPORT_HPP

#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <asps/transport/transport_service.hpp>

namespace asps {
namespace transport {

using namespace boost::asio;
using namespace boost::system;

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
    buffer_.resize(65536);
  }
  virtual ~boost_connection() {}

public:
  virtual void set_read_handler(const read_handler handler) override
  {read_handler_ = handler;}
  virtual void set_write_handler(const write_handler handler) override
  {write_handler_ = handler;}
  virtual void set_close_handler(const close_handler handler) override
  {close_handler_ = handler;}
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
  virtual void set_connect_handler(const connect_handler handler) override
  {connect_handler_ = handler;}
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
  virtual void set_accept_handler(const accept_handler handler) override
  {accept_handler_ = handler;}
  virtual void set_release_handler(const release_handler handler) override
  {release_handler_ = handler;}
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

} // transport
} // asps

#endif // ASPS_TRANSPORT_TRANSPORT_HPP
