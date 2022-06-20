// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Client Message.

#ifndef ASPS_MODBUS_PDU_MESSAGE_CLIENT_MESSAGE_HPP
#define ASPS_MODBUS_PDU_MESSAGE_CLIENT_MESSAGE_HPP

#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>
#include <asps/modbus/pdu/message/message_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

class client_exception
  : public message_unserialization_service
{
public:
  client_exception(message_unserialization_service::pointer_type response)
    : message_unserialization_service(),
      response_(response)
  {}

public:
  virtual bool unserialize(const buffer_type& buffer) override;

public:
  virtual mb_datas& datas() override
  {return response_->datas();}

private:
  message_unserialization_service::pointer_type response_;
};

class client_read_coils_request
  : public message_serialization_service
{
public:
  client_read_coils_request(uint16_t starting_address,
                            uint16_t quantity_of_coils)
    : starting_address_(starting_address),
      quantity_of_coils_(quantity_of_coils)
  {}

public:
  virtual const buffer_type& serialize() override;

private:
  uint16_t starting_address_;
  uint16_t quantity_of_coils_;
  buffer_type buffer_;
};

class client_read_coils_response
  : public message_unserialization_service
{
public:
  client_read_coils_response()
    : message_unserialization_service()
  {}

public:
  virtual bool unserialize(const buffer_type& buffer) override;

public:
  virtual mb_datas& datas() override
  {return status_;}

private:
  coils status_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_MESSAGE_CLIENT_MESSAGE_HPP
