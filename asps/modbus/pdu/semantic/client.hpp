// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus PDU Client.

#ifndef ASPS_MODBUS_PDU_SEMANTIC_CLIENT_HPP
#define ASPS_MODBUS_PDU_SEMANTIC_CLIENT_HPP

#include <functional>
#include <asps/modbus/pdu/semantic/constant.hpp>
#include <asps/modbus/pdu/semantic/data_model.hpp>
#include <asps/modbus/pdu/semantic/adu_service.hpp>
#include <asps/modbus/pdu/semantic/session_service.hpp>

namespace asps {
namespace modbus {
namespace pdu {

using namespace std::placeholders;

// Modbus PDU Client
class client
  : public client_observer
{
public:
  client(adu_service::pointer_type adu = make_tcp_adu())
    : adu_(adu),
      session_(make_client_session())
  {
    session_->register_observer(this);
    adu_->set_handler(std::bind(&client::read_handler, this, _1));
  }
  virtual ~client() {}

public:
  void read_coils(uint16_t starting_address, uint16_t quantity_of_coils);

public:
  virtual void on_read_coils(const coils& status) {}
  virtual void on_exception(function_code fc, exception_code ec) {}

private:
  virtual void update_send(const buffer_type& pdu) override;
  virtual void update_datas(const request::pointer_type& req,
                            const mb_datas& datas) override;
  virtual void update_exception(const request::pointer_type& req,
                                exception_code ec) override;

private:
  void read_handler(const buffer_type& pdu);

protected:
  adu_service::pointer_type adu_;
  client_session_service::pointer_type session_;
};

} // pdu
} // modbus
} // asps

#endif // ASPS_MODBUS_PDU_SEMANTIC_CLIENT_HPP
