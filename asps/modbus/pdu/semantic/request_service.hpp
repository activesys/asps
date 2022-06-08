// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Request Interface.

#ifndef ASPS_MODBUS_PDU_SEMANTIC_REQUEST_SERVICE_HPP
#define ASPS_MODBUS_PDU_SEMANTIC_REQUEST_SERVICE_HPP

#include <cstdint>
#include <memory>
#include <asps/modbus/pdu/semantic/constant.hpp>

namespace asps {
namespace modbus {
namespace pdu {

class request
{
public:
  typedef std::shared_ptr<request> pointer_type;

public:
  virtual ~request() {}

public:
  virtual bool valid() = 0;
  virtual request::pointer_type split() = 0;

public:
  function_code func_code()
  {return fc_;}

protected:
  function_code fc_;
};

request::pointer_type
make_read_coils_request(uint16_t address, uint16_t quantity);

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEMANTIC_REQUEST_SERVICE_HPP
