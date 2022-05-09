# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Set demo version informations
set(ASPS_DEMO_VERSION           0x010000)
set(ASPS_DEMO_VERSION_MAJOR     1)
set(ASPS_DEMO_VERSION_MINOR     0)
set(ASPS_DEMO_VERSION_PATCH     0)
set(ASPS_DEMO_VERSION_TEXT      "${ASPS_DEMO_VERSION_MAJOR}.${ASPS_DEMO_VERSION_MINOR}.${ASPS_DEMO_VERSION_PATCH}")
set(ASPS_DEMO_VERSION_TEXT_DEF  "${ASPS_DEMO_VERSION_MAJOR}.${ASPS_DEMO_VERSION_MINOR}")
set(ASPS_DEMO_SOVERSION_TEXT    "${ASPS_DEMO_VERSION_MAJOR}.${ASPS_DEMO_VERSION_MINOR}")

##################################
# Set demo config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/demo_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/demo_config.h
)

##################################
# Set sources for demo
set(ASPS_DEMO_SOURCES
  api/demo_client.cc
  api/demo_server.cc
  semantic/demo_data.cc
  session/client_session.cc
  session/server_session.cc
  message/client_message.cc
  message/server_message.cc
  utility/timer.cc
  utility/utility.cc
  utility/transport.cc
  config/config.cc
  sequence/client_sequence.cc
  sequence/server_sequence.cc
  sequence/state.cc
  sequence/sequence.cc
)

##################################
# Build shared library
add_library(asdemo SHARED ${ASPS_DEMO_SOURCES})
set_target_properties(asdemo
    PROPERTIES
    VERSION     ${ASPS_DEMO_VERSION_TEXT}
    SOVERSION   ${ASPS_DEMO_SOVERSION_TEXT}
)
target_link_libraries(asdemo Boost::system pthread)

##################################
# Install library
#install(asdemo  LIBRARY DESTINATION lib)
