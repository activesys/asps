// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Data.

#ifndef ASPS_DEMO_SEMANTIC_DEMO_DATA_H
#define ASPS_DEMO_SEMANTIC_DEMO_DATA_H

#include <cstdint>
#include <memory>
#include <vector>
#include <arpa/inet.h>

namespace asps {
namespace demo {

// host sequence to network sequence for 64bits integer.
uint64_t htonll(uint64_t value);

// Demo Data
class demo_data
{
public:
  enum value_type {
    invalid_type = -1,
    boolean_type = 0,
    int8_type = 1,
    uint8_type = 2,
    int16_type = 3,
    uint16_type = 4,
    int32_type = 5,
    uint32_type = 6,
    int64_type = 7,
    uint64_type = 8,
    float32_type = 9,
    float64_type = 10
  };

  typedef std::shared_ptr<demo_data> pointer_type;

public:
  demo_data(value_type vt, uint32_t k, uint64_t t)
    : type_(vt),
      key_(k),
      timestamp_(t)
  {}
  virtual ~demo_data() {}

public:
  value_type type() {return type_;}
  uint32_t key() {return key_;}
  uint64_t timestamp() {return timestamp_;}

public:
  virtual std::size_t size() = 0;
  virtual void assign_network_sequence_value(uint8_t* pos) = 0;

protected:
  value_type type_;
  uint32_t key_;
  uint64_t timestamp_;
};

template <typename T>
demo_data::pointer_type make_demo_data(uint32_t k, T v, uint64_t t)
{
  return std::make_shared<demo_data>(demo_data::invalid_type, k, t);
}

#define MAKE_DEMO_DATA_SPECILICATION(TT, VT, FT)                      \
  class demo_data_##TT : public demo_data                             \
  {                                                                   \
  public:                                                             \
    demo_data_##TT(uint32_t k, TT v, uint64_t t)                      \
      : demo_data(VT, k, t),                                          \
        value_(v)                                                     \
    {}                                                                \
                                                                      \
  public:                                                             \
    virtual std::size_t size() override {return sizeof(TT);}          \
    virtual void assign_network_sequence_value(uint8_t* pos) override \
    {*reinterpret_cast<TT*>(pos) = FT(value_);}                       \
                                                                      \
  private:                                                            \
    TT value_;                                                        \
  };                                                                  \
  template<> inline                                                   \
  demo_data::pointer_type make_demo_data(uint32_t k, TT v, uint64_t t)\
  {return std::make_shared<demo_data_##TT>(k, v, t);}

MAKE_DEMO_DATA_SPECILICATION(bool, boolean_type, bool)
MAKE_DEMO_DATA_SPECILICATION(int8_t, int8_type, int8_t)
MAKE_DEMO_DATA_SPECILICATION(uint8_t, uint8_type, uint8_t)
MAKE_DEMO_DATA_SPECILICATION(int16_t, int16_type, htons)
MAKE_DEMO_DATA_SPECILICATION(uint16_t, uint16_type, htons)
MAKE_DEMO_DATA_SPECILICATION(int32_t, int32_type, htonl)
MAKE_DEMO_DATA_SPECILICATION(uint32_t, uint32_type, htonl)
MAKE_DEMO_DATA_SPECILICATION(int64_t, int64_type, htonll)
MAKE_DEMO_DATA_SPECILICATION(uint64_t, uint64_type, htonll)
MAKE_DEMO_DATA_SPECILICATION(float, float32_type, float)
MAKE_DEMO_DATA_SPECILICATION(double, float64_type, double)

#undef MAKE_DEMO_DATA_SPECILICATION

// Demo data group
typedef std::vector<demo_data::pointer_type> data_group_type;

} // demo
} // asps

#endif // ASPS_DEMO_SEMANTIC_DEMO_DATA_H
