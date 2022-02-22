// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus access model.

#ifndef ASPS_MODBUS_SEMANTIC_ACCESSMODEL_H
#define ASPS_MODBUS_SEMANTIC_ACCESSMODEL_H

#include <cstdint>
#include <algorithm>
#include <deque>
#include <memory>
#include <stdexcept>

#include <asps/modbus/semantic/constant.h>

namespace asps {
namespace modbus {

// Modbus Access Model
/**
 * Read Only Access Model
 */
template <typename T>
class read_only_model
{
public:
  typedef std::shared_ptr<read_only_model<T>> ptr_type;

public:
  read_only_model(uint16_t starting_address, uint16_t count, const T* datas)
    : starting_address_(starting_address),
      count_(count),
      datas_(datas),
      code_(success)
  {}
  virtual ~read_only_model() {}

public:
  const T& operator[](uint16_t address) const noexcept
  {
    return datas_[address - starting_address_];
  }

  const T& at(uint16_t address) const
  {
    if (datas_ == nullptr ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address");
    }

    return datas_[address - starting_address_];
  }

  uint16_t starting_address() const {return starting_address_;}
  uint16_t count() const {return count_;}
  exception_code code() const {return code_;}
  void code(exception_code code) {code_ = code;}

public:
  static void split(
    const read_only_model& model, uint16_t count, std::deque<ptr_type>& models)
  {
    for (uint16_t i = 0; i < model.count_; i += count) {
      models.push_back(std::make_shared<read_only_model<T>>(
        model.starting_address_ + i,
        std::min<uint16_t>(count, model.count_ - i),
        model.datas_ + i));
    }
  }

protected:
  exception_code code_;
  uint16_t starting_address_;
  uint16_t count_;
  const T* datas_;
};

/**
 * Read Write Access Model
 */
template <typename T>
class read_write_model
{
public:
  typedef std::shared_ptr<read_write_model<T>> ptr_type;

public:
  read_write_model(uint16_t starting_address, uint16_t count, T* datas)
    : starting_address_(starting_address),
      count_(count),
      datas_(datas),
      code_(success)
  {}
  virtual ~read_write_model() {};

public:
  T& operator[](uint16_t address) noexcept
  {
    return datas_[address - starting_address_];
  }
  const T& operator[](uint16_t address) const noexcept
  {
    return datas_[address - starting_address_];
  }

  T& at(uint16_t address)
  {
    if (datas_ == nullptr ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address");
    }

    return datas_[address - starting_address_];
  }

  const T& at(uint16_t address) const
  {
    if (datas_ == nullptr ||
        address < starting_address_ ||
        address >= starting_address_ + count_) {
      throw std::out_of_range("Invalid address");
    }

    return datas_[address - starting_address_];
  }

  uint16_t starting_address() const {return starting_address_;}
  uint16_t count() const {return count_;}
  exception_code code() const {return code_;}
  void code(exception_code code) {code_ = code;}

public:
  static void split(
    const read_write_model& model, uint16_t count, std::deque<ptr_type>& models)
  {
    for (uint16_t i = 0; i < model.count_; i += count) {
      models.push_back(std::make_shared<read_write_model<T>>(
        model.starting_address_ + i,
        std::min<uint16_t>(count, model.count_ - i),
        model.datas_ + i));
    }
  }

protected:
  exception_code code_;
  uint16_t starting_address_;
  uint16_t count_;
  T* datas_;
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SEMANTIC_ACCESSMODEL_H
