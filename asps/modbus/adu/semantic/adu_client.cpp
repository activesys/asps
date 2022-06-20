// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Client.

#include <asps/modbus/adu/semantic/adu_client.hpp>

namespace asps {
namespace modbus {
namespace adu {

// Modbus AUD Client
void client::update_send(const buffer_type& adu)
{
  connection_->write(adu);
}

void client::update_pdu(const buffer_type& pdu)
{
  handler_(pdu);
}

void client::set_handler(read_handler rhandler)
{
  handler_ = rhandler;
}

void client::write(const buffer_type& pdu)
{
  session_.send_request(pdu);
}

void client::on_read(connection::pointer_type conn,
                     const buffer_type& buffer,
                     std::size_t bytes)
{
  session_.receive_response(buffer);
}

void client::on_write(connection::pointer_type conn,
                      std::size_t bytes)
{}

void client::on_close(connection::pointer_type conn)
{}

} // adu
} // modbus
} // asps
