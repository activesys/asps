// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo State.

#ifndef ASPS_DEMO_SEQUENCE_STATE_H
#define ASPS_DEMO_SEQUENCE_STATE_H

#include <cstdint>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

class positive_keepalive_sequence;
class state
{
public:
  virtual ~state() {}

public:
  virtual const buffer_type& request(positive_keepalive_sequence* seq) = 0;
  virtual bool response(positive_keepalive_sequence* seq,
                        const uint8_t* buffer) = 0;
  virtual void timeout(positive_keepalive_sequence* seq) = 0;
};

class none_state : public state
{
private:
  none_state() = default;

public:
  virtual const buffer_type& request(positive_keepalive_sequence* seq) override;
  virtual bool response(positive_keepalive_sequence* seq,
                        const uint8_t* buffer) override;
  virtual void timeout(positive_keepalive_sequence* seq) override;

public:
  static state* instance()
  {
    if (instance_ == nullptr) {
      instance_ = new none_state();
    }

    return instance_;
  }

private:
  static state* instance_;
};

class keepalive_sent_state : public state
{
private:
  keepalive_sent_state() = default;

public:
  virtual const buffer_type& request(positive_keepalive_sequence* seq) override;
  virtual bool response(positive_keepalive_sequence* seq,
                        const uint8_t* buffer) override;
  virtual void timeout(positive_keepalive_sequence* seq) override;

public:
  static state* instance()
  {
    if (instance_ == nullptr) {
      instance_ = new keepalive_sent_state();
    }

    return instance_;
  }

private:
  static state* instance_;
  buffer_type empty_buffer_;
};

} // demo
} // asps

#endif // ASPS_DEMO_SEQUENCE_STATE_H
