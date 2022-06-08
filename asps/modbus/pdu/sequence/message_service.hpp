// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Message service.

#ifndef ASPS_MODBUS_PDU_SEQUENCE_MESSAGE_SERVICE_HPP
#define ASPS_MODBUS_PDU_SEQUENCE_MESSAGE_SERVICE_HPP

#include <memory>
#include <asps/utility/utility.h>
#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>

namespace asps {
namespace modbus {
namespace pdu {

using namespace asps::utility;

class message_serialization_service
{
public:
  typedef std::shared_ptr<message_serialization_service> pointer_type;

public:
  virtual ~message_serialization_service() {}

public:
  virtual const buffer_type& serialize() = 0;
};

class message_unserialization_service
{
public:
  typedef std::shared_ptr<message_unserialization_service> pointer_type;

public:
  message_unserialization_service()
    : fc_(function_code_invalid_pdu),
      ec_(exception_code_success)
  {}
  virtual ~message_unserialization_service() {}

public:
  virtual bool unserialize(const buffer_type& buffer) = 0;

public:
  virtual mb_datas& datas() = 0;

public:
  function_code func_code()
  {return fc_;}
  exception_code excep_code()
  {return ec_;}

protected:
  function_code fc_;
  exception_code ec_;
};

message_serialization_service::pointer_type
make_client_read_coils_request(uint16_t starting_address,
                               uint16_t quantity_of_coils);
message_unserialization_service::pointer_type
make_client_read_coils_response();

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEQUENCE_MESSAGE_SERVICE_HPP
