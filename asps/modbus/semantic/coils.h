// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Coils and Discrete Inputs.

#ifndef ASPS_MODBUS_SEMANTIC_COILS_H
#define ASPS_MODBUS_SEMANTIC_COILS_H

#include <memory>
#include <vector>
#include <asps/modbus/semantic/constant.h>
#include <asps/modbus/semantic/access_model.h>

namespace asps {
namespace modbus {

// Modbus Coils
#if 0
class coils : public read_write_model<bit_type>
{
public:
  coils(uint16_t starting_address, uint16_t count, const bits_type& status)
    : read_write_model(starting_address, count, status)
  {}
  coils(uint16_t starting_address, uint16_t count, const bit_type* status = nullptr)
    : read_write_model(starting_address, count, status)
  {}
};
#endif
class coils
{
public:
  typedef std::shared_ptr<coils> pointer_type;

public:
  coils(uint16_t starting_address, uint16_t count, const bits_type& status)
    : starting_address_(starting_address),
      count_(count),
      status_(status),
      code_(success)
  {}
  coils(uint16_t starting_address, uint16_t count, const bit_type* status = nullptr)
    : starting_address_(starting_address),
      count_(count),
      code_(success)
  {
    if (status) {
      status_.assign(status, status + count);
    } else {
      status_.resize(count);
    }
  }

public:
  template <typename OutputIterator>
  static void split(const coils& cs, uint16_t count, OutputIterator pos)
  {
    std::vector<coils::pointer_type> csv;

    for (uint16_t i = 0; i < cs.count_; i += count) {
      uint16_t starting_address = cs.starting_address_ + i;
      uint16_t min_count = std::min<uint16_t>(count, cs.count_ - i);
      bits_type status(cs.status_.begin() + i, cs.status_.begin() + i + min_count);
      csv.push_back(std::make_shared<coils>(starting_address, min_count, status));
    }

    std::move(csv.begin(), csv.end(), pos);
  }

public:
  bool bit(uint16_t address) const;
  void bit(uint16_t address, bool s);

  const bits_type& status() const {return status_;}
  void status(const bits_type& s) {status_ = s;}
  uint16_t starting_address() const {return starting_address_;}
  uint16_t count() const {return count_;}
  exception_code code() const {return code_;}
  void code(exception_code code) {code_ = code;}

private:
  exception_code code_;
  uint16_t starting_address_;
  uint16_t count_;
  bits_type status_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SEMANTIC_COILS_H
