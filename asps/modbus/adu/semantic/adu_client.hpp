// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Client.

#ifndef ASPS_MODBUS_ADU_SEMANTIC_ADU_CLIENT_HPP
#define ASPS_MODBUS_ADU_SEMANTIC_ADU_CLIENT_HPP

#include <functional>
#include <asps/transport/transport_service.hpp>
#include <asps/modbus/frame/adu_service.hpp>
#include <asps/modbus/adu/session/client_session.hpp>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::transport;
using namespace asps::utility;
using namespace std::placeholders;

class adu_client
  : public client_session_observer,
    public frame::adu_service
{
public:
  adu_client(connection::pointer_type conn)
    : connection_(conn)
  {
    session_.register_observer(this);

    connection_->set_read_handler(std::bind(&adu_client::connection_read_handler,
                                            this, _1, _2, _3));
    connection_->set_write_handler(std::bind(&adu_client::connection_write_handler,
                                             this, _1, _2));
    connection_->set_close_handler(std::bind(&adu_client::connection_close_handler,
                                             this, _1));
  }

private:
  void connection_read_handler(connection::pointer_type conn,
                               const buffer_type& buffer,
                               std::size_t bytes);
  void connection_write_handler(connection::pointer_type conn,
                                std::size_t bytes);
  void connection_close_handler(connection::pointer_type conn);

public:
  virtual void update_send(const buffer_type& adu) override;
  virtual void update_pdu(const buffer_type& pdu) override;

public:
  virtual void set_handler(read_handler rhandler) override;
  virtual void write(const buffer_type& pdu) override;

private:
  connection::pointer_type connection_;
  client_session session_;
  read_handler handler_;
};

} // adu
} // modbus
} // asps

#endif // ASPS_MODBUS_ADU_SEMANTIC_ADU_CLIENT_HPP
