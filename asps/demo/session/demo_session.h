// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Session.

#ifndef ASPS_DEMO_MESSAGE_DEMO_SESSION_H
#define ASPS_DEMO_MESSAGE_DEMO_SESSION_H

#include <cstdint>
#include <vector>

#include <asps/demo/message/demo_message.h>

namespace asps {
namespace demo {

// Demo Session
class demo_session
{
public:
  demo_session(bool compress_same_type,
               bool compress_same_timestamp,
               bool compress_key_sequence)
    : compress_same_type_(compress_same_type),
      compress_same_timestamp_(compress_same_timestamp),
      compress_key_sequence_(compress_key_sequence)
  {}

public:
  void compress_same_type(bool flag) {compress_same_type_ = flag;}
  void compress_same_timestamp(bool flag) {compress_same_timestamp_ = flag;}
  void compress_key_sequence(bool flag) {compress_key_sequence_ = flag;}

  template <typename IT>
  const std::vector<uint8_t> send(IT first, IT second)
  {
    demo_message::pointer_type dm =
      make_demo_message(first,
                        second,
                        compress_same_type_,
                        compress_same_timestamp_,
                        compress_key_sequence_);
    return dm->serialize();
  }

private:
  bool compress_same_type_;
  bool compress_same_timestamp_;
  bool compress_key_sequence_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_SESSION_H
