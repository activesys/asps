// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Server.

#include <iostream>
#include <iomanip>
#include <example/demo/options_parser.h>
#include <asps/demo/demo.h>

using namespace asps::demo;
using namespace asps_example::demo_example;

class my_demo_server
  : public demo_server
{
public:
  my_demo_server(uint16_t port, bool debug = false)
    : demo_server(port),
      debug_(debug)
  {}

public:
  void on_accept(const connection::pointer_type conn) override
  {
    std::cout << std::endl
              << "demo_server> establish a connection to "
              << conn->remote_address() << ":"
              << conn->remote_port() << std::endl;
  }
  void on_read(const connection::pointer_type conn,
               const data_group_type& datas) override
  {
    std::cout << std::endl
              << "demo_server> receive datas from "
              << conn->remote_address() << ":"
              << conn->remote_port() << std::endl;
    for (auto d : datas) {
      std::cout << "demo_server> key: " << d->key()
                << " timestamp: " << d->timestamp();
      switch (d->type()) {
      case demo_data::boolean_type:
        {
          bool v;
          d->get_value(&v);
          std::cout << " value: " << std::boolalpha << v << std::endl;
        }
        break;
      case demo_data::int8_type:
        {
          int8_t v;
          d->get_value(&v);
          std::cout << " value: " << int16_t(v) << std::endl;
        }
        break;
      case demo_data::uint8_type:
        {
          uint8_t v;
          d->get_value(&v);
          std::cout << " value: " << uint16_t(v) << std::endl;
        }
        break;
      case demo_data::int16_type:
        {
          int16_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::uint16_type:
        {
          uint16_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::int32_type:
        {
          int32_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::uint32_type:
        {
          uint32_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::int64_type:
        {
          int64_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::uint64_type:
        {
          uint64_t v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::float32_type:
        {
          float v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      case demo_data::float64_type:
        {
          double v;
          d->get_value(&v);
          std::cout << " value: " << v << std::endl;
        }
        break;
      default:
        std::cout << " invalid type: " << d->type() << std::endl;
        break;
      }
    }
  }
  void on_read_raw(const connection::pointer_type conn,
                   const buffer_type& buffer,
                   std::size_t bytes) override
  {
    if (debug_) {
      std::cout << std::endl
                << "demo_server> receive raw datas from "
                << conn->remote_address() << ":"
                << conn->remote_port() << std::endl;
      std::cout << "demo_server>";
      for (std::size_t i = 0; i < bytes; ++i) {
        if (i != 0 && (i % 16 == 0)) {
          std::cout << std::endl << "demo_server>";
        }
        std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << uint16_t(buffer[i]);
      }
      std::cout << std::dec << std::endl;
    }
  }
  void on_write_raw(const connection::pointer_type conn,
                   const buffer_type& buffer,
                   std::size_t bytes) override
  {
    if (debug_) {
      std::cout << std::endl
                << "demo_server> send raw datas to "
                << conn->remote_address() << ":"
                << conn->remote_port() << std::endl;
      std::cout << "demo_server>";
      for (std::size_t i = 0; i < bytes; ++i) {
        if (i != 0 && (i % 16 == 0)) {
          std::cout << std::endl << "demo_server>";
        }
        std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << uint16_t(buffer[i]);
      }
      std::cout << std::dec << std::endl;
    }
  }
  void on_close(const connection::pointer_type conn) override
  {
    std::cout << std::endl
              << "demo_server> disconnect the connection to "
              << conn->remote_address() << ":"
              << conn->remote_port() << std::endl;
  }

private:
  bool debug_;
};

int main(int argc, char* argv[])
{
  server_options_parser sop(argc, argv);
  if (!sop) {
    sop.usage();
    return 1;
  }

  config::t1_t2(sop.t1(), sop.t2());
  my_demo_server server(sop.port(), sop.debug());
  std::cout << "demo_server> listen on " << sop.port() << std::endl;
  server.run();

  return 0;
}

