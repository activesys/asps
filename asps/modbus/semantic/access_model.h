// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus access model.

#ifndef ASPS_MODBUS_SEMANTIC_ACCESS_MODEL_H
#define ASPS_MODBUS_SEMANTIC_ACCESS_MODEL_H

#include <cstdint>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include <asps/modbus/semantic/constant.h>

namespace asps {
namespace modbus {

// Modbus Memory Accessing Model
template <typename T>
class accessing_model
{
public:
  typedef T value_type;
  typedef std::vector<value_type> memory_type;
  typedef std::shared_ptr<accessing_model<T>> pointer_type;

public:
  accessing_model(
    uint16_t starting_address,
    uint16_t count,
    const memory_type& memory)
    : starting_address_(starting_address),
      count_(count),
      memory_(memory),
      code_(success)
  {}
  accessing_model(
    uint16_t starting_address,
    uint16_t count,
    const value_type* values = nullptr)
    : starting_address_(starting_address),
      count_(count),
      code_(success)
  {
    if (values) {
      memory_.assign(values, values + count);
    } else {
      memory_.resize(count);
    }
  }
  virtual ~accessing_model() {}

public:
  virtual T value(uint16_t address) const = 0;
  virtual void value(uint16_t address, T v) = 0;

public:
  uint16_t starting_address() const {return starting_address_;}
  uint16_t count() const {return count_;}
  exception_code code() const {return code_;}
  void code(exception_code code) {code_ = code;}

public:
  template <typename OutputIterator>
  static void split(const accessing_model& model,
                    uint16_t count, OutputIterator pos)
  {
    std::vector<pointer_type> models;

    for (uint16_t i = 0; i < model.count_; i += count) {
      uint16_t starting_address = model.starting_address_ + i;
      uint16_t min_count = std::min<uint16_t>(count, model.count_ - i);
      memory_type memory(model.memory_.begin() + i,
                         model.memory_.begin() + i + min_count);
      models.push_back(std::make_shared<accessing_model<value_type>>(
        starting_address, min_count, memory));
    }

    std::move(models.begin(), models.end(), pos);
  }

protected:
  exception_code code_;
  uint16_t starting_address_;
  uint16_t count_;
  memory_type memory_;
};

/**
 * Read Only Access Model
 */
template <typename T>
class read_only_model : public accessing_model<T>
{
public:
  read_only_model(
    uint16_t starting_address,
    uint16_t count,
    const typename accessing_model<T>::memory_type& memory)
    : accessing_model<T>(starting_address, count, memory)
  {}
  read_only_model(
    uint16_t starting_address,
    uint16_t count,
    const typename accessing_model<T>::value_type* values = nullptr)
    : accessing_model<T>(starting_address, count, values)
  {}
  virtual ~read_only_model() {}

public:
  virtual T value(uint16_t address) const override
  {
    if (accessing_model<T>::memory_.size() < accessing_model<T>::count_ ||
        address < accessing_model<T>::starting_address_ ||
        address >= accessing_model<T>::starting_address_ + accessing_model<T>::count_) {
      throw std::out_of_range("Invalid address");
    }

    return accessing_model<T>::memory_[address - accessing_model<T>::starting_address_];
  }

  virtual void value(uint16_t address, T v) override
  {
    throw std::logic_error("Modify data in read-only model");
  }
};

/**
 * Read Write Access Model
 */
template <typename T>
class read_write_model : public read_only_model<T>
{
public:
  read_write_model(
    uint16_t starting_address,
    uint16_t count,
    const typename accessing_model<T>::memory_type& memory)
    : read_only_model<T>(starting_address, count, memory)
  {}
  read_write_model(
    uint16_t starting_address,
    uint16_t count,
    const typename accessing_model<T>::value_type* values = nullptr)
    : read_only_model<T>(starting_address, count, values)
  {}
  virtual ~read_write_model() {}

public:
  void value(uint16_t address, T v) override
  {
    if (accessing_model<T>::memory_.size() < accessing_model<T>::count_ ||
        address < accessing_model<T>::starting_address_ ||
        address >= accessing_model<T>::starting_address_ + accessing_model<T>::count_) {
      throw std::out_of_range("Invalid address");
    }

    accessing_model<T>::memory_[address - accessing_model<T>::starting_address_] = v;
  }
};

} // namespace modbus
} // namespace asps

#endif // ASPS_MODBUS_SEMANTIC_ACCESS_MODEL_H
