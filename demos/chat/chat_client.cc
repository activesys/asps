// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Chat Client.

#include <iostream>
#include <asps/chat/client.h>

using namespace asps::chat;

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: chat_client host port" << std::endl;
    return 1;
  }

  client_notify notify{
    [](uint16_t id, const std::string& msg)
    {
      std::cout << "chat id: " << id << ", message: " << msg << std::endl;
    },
    [](const std::string& err_msg)
    {
      std::cerr << "error message: " << err_msg << std::endl;
    }};

  client c(argv[1], std::atoi(argv[2]), notify);
  if (!c.connect()) {
    std::cerr << "Connect to " << argv[1] << ":" << argv[2] << " failed" << std::endl;
    return 1;
  }

  std::string msg;
  std::getline(std::cin, msg);
  c.send(msg);
  std::getline(std::cin, msg);
  c.send(msg);
  std::getline(std::cin, msg);
  c.send(msg);

  c.close();

  return 0;
}
