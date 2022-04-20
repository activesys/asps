// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Transport service.

#ifndef ASPS_DEMO_API_TRANSPORT_SERVICE_H
#define ASPS_DEMO_API_TRANSPORT_SERVICE_H

#include <memory>
#include <functional>
#include <asps/demo/semantic/demo_data.h>

namespace asps {
namespace demo {

class transport_service
{
public:
  typedef std::shared_ptr<transport_service> pointer_type;

  typedef std::function<void(bool)> connect_handler;
  typedef std::function<void(bool,std::size_t)> write_handler;
  typedef std::function<void(bool,const buffer_type&,std::size_t)> read_handler;

public:
  transport_service() = default;
  virtual ~transport_service() {}

public:
  virtual void read(const read_handler& handler) = 0;
  virtual void connect(const connect_handler& handler) = 0;
  virtual void close() = 0;
  virtual void write(const buffer_type& buf, const write_handler& handler) = 0;
  virtual void run() = 0;
  virtual void stop() = 0;
};

transport_service::pointer_type make_transport_service();

} // demo
} // asps

#endif // ASPS_DEMO_API_TRANSPORT_SERVICE_H
