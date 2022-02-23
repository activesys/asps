// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Modbus Server.

#include <iostream>
#include <asps/modbus/modbus.h>

using namespace asps::modbus;

class my_event : public server_event
{
public:
  my_event(server& s)
    : server_event(s)
  {
    for (uint16_t i = 0; i < 0xffff; ++i) {
      my_memory_[i] = i%2;
    }
  }

public:
  void on_accept(const std::string& address, uint16_t port) override
  {
    std::cout << "modbus_server> accept connection from " 
              << address << ":" << port << std::endl;
  }
  void on_accept(const std::string& error_message) override
  {
    std::cout << "modbus_server> accept error: " << error_message << std::endl;
  }
  void on_error(const std::string& error_message) override
  {
    std::cout << "modbus_server> error: " << error_message << std::endl;
  }
  coils::pointer_type on_read_coils(const coils::pointer_type cs) override
  {
    std::cout << "modbus_server> read_coils: starting_address " 
              << cs->starting_address() << ", "
              << "quantity_of_coils " << cs->count() << std::endl;

    return std::make_shared<coils>(
      cs->starting_address(), cs->count(), my_memory_+cs->starting_address());
  }
  coils::pointer_type on_write_single_coil(const coils::pointer_type cs) override
  {
    return std::make_shared<coils>(
      cs->starting_address(), cs->count(), my_memory_+cs->starting_address());
  }
  coils::pointer_type on_write_multiple_coils(const coils::pointer_type cs) override
  {
    return std::make_shared<coils>(
      cs->starting_address(), cs->count(), my_memory_+cs->starting_address());
  }

private:
  static bool my_memory_[0xffff];
};

bool my_event::my_memory_[0xffff];

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: modbus_server port" << std::endl;
    return 1;
  }

  server s(std::atoi(argv[1]));
  s.register_event(new my_event(s));
  s.async_listen();
  s.run();

  return 0;
}
