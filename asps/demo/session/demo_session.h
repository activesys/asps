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
               bool compress_same_timestamp)
    : compress_same_type_(compress_same_type),
      compress_same_timestamp_(compress_same_timestamp)
  {}

public:
  void compress_same_type(bool flag) {compress_same_type_ = flag;}
  void compress_same_timestamp(bool flag) {compress_same_timestamp_ = flag;}

  template <typename IT>
  const std::vector<uint8_t> send(IT first, IT second)
  {
    demo_message dm(first,
                    second,
                    compress_same_type_,
                    compress_same_timestamp_);
    return dm.serialize();
  }

private:
  bool compress_same_type_;
  bool compress_same_timestamp_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_SESSION_H
