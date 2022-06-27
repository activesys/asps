// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Client.

#include <asps/modbus/adu/semantic/adu_client.hpp>

namespace asps {
namespace modbus {
namespace adu {

using namespace asps::modbus::frame;

// Make adu service
adu_service::pointer_type make_tcp_adu(connection::pointer_type conn)
{
  return std::make_shared<adu_client>(conn);
}

// Modbus AUD Client
void adu_client::update_send(const buffer_type& adu)
{
  connection_->write(adu);
}

void adu_client::update_pdu(const buffer_type& pdu)
{
  handler_(pdu);
}

void adu_client::set_handler(read_handler rhandler)
{
  handler_ = rhandler;
}

void adu_client::write(const buffer_type& pdu)
{
  session_.send_request(pdu);
}

void adu_client::connection_read_handler(connection::pointer_type conn,
                                         const buffer_type& buffer,
                                         std::size_t bytes)
{
  session_.receive_response(buffer);
}

void adu_client::connection_write_handler(connection::pointer_type conn,
                                          std::size_t bytes)
{}

void adu_client::connection_close_handler(connection::pointer_type conn)
{}

} // adu
} // modbus
} // asps
