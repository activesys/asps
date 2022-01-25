// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Modbus Client.

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <asps/modbus/modbus.h>

using namespace asps::modbus;

class my_event : public client_event
{
public:
  my_event(client& c)
    : client_event(c)
  {}

public:
  void on_connect(const std::string& address, uint16_t port) override
  {
    std::cout << "modbus_client> connect to " << address << ":" << port << std::endl;
    get_cmd();
  }
  void on_connect(const std::string& error_message) override
  {
    std::cerr << "modbus_client> failed to connect server. error message: "
              << error_message << std::endl;
    modbus_client.async_connect();
  }
  void on_error(const std::string& error_message) override
  {
    std::cerr << "modbus_client> error message: "
              << error_message << std::endl;
  }

  void on_read_coils(const coils::ptr_type cs, exception_code code) override
  {
    if (code) {
      std::cout << "modbus_client> read coils response, starting address: "
                << cs->starting_address()
                << " quantity of coils: "
                << cs->count()
                << " exception code: "
                << code
                << std::endl;
    } else {
      std::cout << "modbus_client> read coils response, starting address: "
                << cs->starting_address()
                << " quantity of coils: "
                << cs->count()
                << std::endl;
      for (uint16_t i = 0; i < cs->count(); ++i) {
        std::cout << "[" << std::setfill('0') << std::setw(5) << std::right
                  << cs->starting_address() + i << "] "
                  << (cs->at(cs->starting_address()+i) ? "on" : "off")
                  << std::endl;
      }
    }
  
    get_cmd();
  }

private:
  void get_cmd()
  {
    std::cout << "modbus_client> command: 1) read coils" << std::endl
              << "modbus_client> Please input command: ";
    uint16_t cmd;
    std::cin >> cmd;
    if (cmd == 1) {
      std::cout << "modbus_client> unit identifier(0~65535): ";
      uint16_t uid;
      std::cin >> uid;
      std::cout << "modbus_client> starting address(0~65535): ";
      uint16_t address;
      std::cin >> address;
      std::cout << "modbus_client> quantity of coils(1~65535): ";
      uint16_t quantity;
      std::cin >> quantity;
      coils cs(address, quantity, my_memory_+address);
      modbus_client.read_coils(uid, cs);
    } else {
      modbus_client.close();
      coils cs();
    }
  }

private:
  static bool my_memory_[0xffff];
};

bool my_event::my_memory_[0xffff];

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: modbus_client host port" << std::endl;
    return 1;
  }

  client c(argv[1], std::atoi(argv[2]));
  c.register_event(new my_event(c));
  c.async_connect();
  c.run();

  return 0;
}
