// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU State.

#ifndef ASPS_MODBUS_PDU_SEQUENCE_STATE_HPP
#define ASPS_MODBUS_PDU_SEQUENCE_STATE_HPP

#include <asps/utility/utility.hpp>

namespace asps {
namespace modbus {
namespace pdu {

using namespace asps::utility;

class active_sequence_service;
class state
{
public:
  virtual ~state() {}

public:
  virtual const buffer_type& request(active_sequence_service* seq) = 0;
  virtual bool response(active_sequence_service* seq,
                        const buffer_type& buffer) = 0;
};

class idle_state : public state
{
public:
  virtual const buffer_type& request(active_sequence_service* seq) override;
  virtual bool response(active_sequence_service* seq,
                        const buffer_type& buffer) override;

public:
  static state* instance()
  {
    if (instance_ == nullptr) {
      instance_ = new idle_state();
    }

    return instance_;
  }

private:
  static state* instance_;
};

class sent_state : public state
{
public:
  virtual const buffer_type& request(active_sequence_service* seq) override;
  virtual bool response(active_sequence_service* seq,
                        const buffer_type& buffer) override;

public:
  static state* instance()
  {
    if (instance_ == nullptr) {
      instance_ = new sent_state();
    }

    return instance_;
  }

private:
  static state* instance_;
  buffer_type empty_buffer_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEQUENCE_STATE_HPP
