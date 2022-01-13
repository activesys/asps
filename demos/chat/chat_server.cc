// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Chat Server.

#include <iostream>
#include <asps/chat/server.h>

using namespace asps::chat;

class my_event : public server_event
{
public:
  my_event(server& s)
    : server_event(s)
  {}

public:
  void on_accept(const std::string& address, uint16_t port) override
  {
    std::cout << "accept connection from " << address 
              << ":" << port << std::endl;
  }

  void on_accept(const std::string& err_msg) override
  {
    std::cerr << "failed accept connection. error message: "
              << err_msg << std::endl;
  }

  std::string on_receive(uint16_t chat_id, const std::string& req_msg) override
  {
    std::cout << chat_id << "<: " << req_msg << std::endl;
    if (req_msg == "bye") {
      return req_msg;
    } else {
      std::string m;
      std::getline(std::cin, m);
      std::cout << chat_id << ">: " << m << std::endl;
      return m;
    }
  }

  void on_error(const std::string& err_msg) override
  {
    std::cerr << "error message: " << err_msg << std::endl;
  }
};

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: chat_server port" << std::endl;
    return 1;
  }

  server s(std::atoi(argv[1]));
  s.register_event(new my_event(s));
  s.async_listen();
  s.run();

  return 0;
}
