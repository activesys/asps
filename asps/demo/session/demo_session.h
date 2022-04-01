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
  demo_session(bool same_type,
               bool key_sequence,
               bool same_timestamp)
    : same_type_(same_type),
      key_sequence_(key_sequence),
      same_timestamp_(same_timestamp)
  {}

public:
  void same_type(bool flag) {same_type_ = flag;}
  void key_sequence(bool flag) {key_sequence_ = flag;}
  void same_timestamp(bool flag) {same_timestamp_ = flag;}

  template <typename IT>
  const std::vector<uint8_t> send(IT first, IT second)
  {
    demo_message dm(first,
                    second,
                    same_type_,
                    key_sequence_,
                    same_timestamp_);
    return dm.serialize();
  }

private:
  bool same_type_;
  bool key_sequence_;
  bool same_timestamp_;
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_DEMO_SESSION_H
