// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus ADU Service.

#ifndef ASPS_MODBUS_FRAME_ADU_SERVICE_HPP
#define ASPS_MODBUS_FRAME_ADU_SERVICE_HPP

#include <memory>
#include <functional>
#include <asps/utility/utility.h>

namespace asps {
namespace modbus {
namespace frame {

using namespace asps::utility;

// ADU Service
class adu_service
{
public:
  typedef std::shared_ptr<adu_service> pointer_type;

  typedef std::function<void(const buffer_type&)> read_handler;
public:
  virtual ~adu_service() {}

public:
  virtual void set_handler(read_handler rhandler) = 0;
  virtual void write(const buffer_type& pdu) = 0;
};

adu_service::pointer_type
make_tcp_adu();
adu_service::pointer_type
make_serial_line_adu();

} // frame
} // modbus
} // asps

#endif // ASPS_MODBUS_FRAME_ADU_SERVICE_HPP
