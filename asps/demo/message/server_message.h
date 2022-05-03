// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Server Message.

#ifndef ASPS_DEMO_MESSAGE_SERVER_MESSAGE_H
#define ASPS_DEMO_MESSAGE_SERVER_MESSAGE_H

#include <asps/demo/utility/utility.h>
#include <asps/demo/semantic/demo_data.h>
#include <asps/demo/sequence/message_service.h>

namespace asps {
namespace demo {

class server_data : public message_unserialization_service
{
public:
  virtual bool unserialize(buffer_type& buffer) override;

private:
  std::size_t unserialization_header_length();

  bool unserialize_header(const uint8_t* pos, std::size_t& length);
  bool unserialize_mutable(const uint8_t* pos, std::size_t& length);
  bool unserialize_datas(const uint8_t* pos, std::size_t& length);
  bool unserialize_one_data(const uint8_t* pos, std::size_t& length);

private:
  uint16_t length_;
  uint16_t count_;
  uint8_t attribute_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_SERVER_MESSAGE_H
