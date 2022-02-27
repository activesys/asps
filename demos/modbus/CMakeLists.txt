# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Build modbus demos executable
add_executable(modbus_client modbus_client.cc)
target_link_libraries(modbus_client asmodbus Boost::system pthread)

#add_executable(modbus_server modbus_server.cc)
#target_link_libraries(modbus_server asmodbus)

