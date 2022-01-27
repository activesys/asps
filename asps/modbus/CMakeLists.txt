# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Set modbus version informations
set(ASPS_MODBUS_VERSION           0x010000)
set(ASPS_MODBUS_VERSION_MAJOR     1)
set(ASPS_MODBUS_VERSION_MINOR     0)
set(ASPS_MODBUS_VERSION_PATCH     0)
set(ASPS_MODBUS_VERSION_TEXT      "${ASPS_MODBUS_VERSION_MAJOR}.${ASPS_MODBUS_VERSION_MINOR}.${ASPS_MODBUS_VERSION_PATCH}")
set(ASPS_MODBUS_VERSION_TEXT_DEF  "${ASPS_MODBUS_VERSION_MAJOR}.${ASPS_MODBUS_VERSION_MINOR}")
set(ASPS_MODBUS_SOVERSION_TEXT    "${ASPS_MODBUS_VERSION_MAJOR}.${ASPS_MODBUS_VERSION_MINOR}")

##################################
# Set modbus config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/modbus_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/modbus_config.h
)

##################################
# Set sources for modbus
set(ASPS_MODBUS_SOURCES
    api/client.cc
    api/server.cc
    semantic/coils.cc
    semantic/registers.cc
    session/session.cc
    pdu/message/pdu.cc
    pdu/sequence/pdu_sequence.cc
    adu/message/tcp_adu.cc
    adu/sequence/tcp_adu_sequence.cc
)

##################################
# Build shared library
add_library(asmodbus SHARED ${ASPS_MODBUS_SOURCES})
set_target_properties(asmodbus
    PROPERTIES
    VERSION     ${ASPS_MODBUS_VERSION_TEXT}
    SOVERSION   ${ASPS_MODBUS_SOVERSION_TEXT}
)
target_link_libraries(asmodbus Boost::system pthread)

##################################
# Install library
#install(asmodbus  LIBRARY DESTINATION lib)
