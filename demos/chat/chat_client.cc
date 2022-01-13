// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Chat Client.

#include <iostream>
#include <asps/chat/client.h>

using namespace asps::chat;

class my_event : public client_event
{
public:
  my_event(client& c)
    : client_event(c)
  {}

public:
  void on_receive(uint16_t chat_id, const std::string& msg) override
  {
    std::cout << chat_id << "<: " << msg << std::endl;
    if (msg == "bye") {
      chat_client.close();
    } else {
      std::string m;
      std::getline(std::cin, m);
      std::cout << chat_client.async_send(m) << ">: " << m << std::endl;
    }
  }

  void on_error(const std::string& err_msg) override
  {
    std::cerr << "error message: " << err_msg << std::endl;
  }

  void on_connect(const std::string& address, uint16_t port) override
  {
    std::cout << "connect to " << address << ":" << port << std::endl;
    std::string msg;
    std::getline(std::cin, msg);
    std::cout << chat_client.async_send(msg) << ">: " << msg << std::endl;
  }

  void on_connect(const std::string& err_msg)
  {
    std::cerr << "failed to connect server. error message: "
              << err_msg << std::endl;
    chat_client.async_connect();
  }
};

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: chat_client host port" << std::endl;
    return 1;
  }

  client c(argv[1], std::atoi(argv[2]));
  c.register_event(new my_event(c));
  c.async_connect();
  c.run();

  return 0;
}
