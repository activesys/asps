# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Set CHAT version informations
set(ASPS_CHAT_VERSION           0x010000)
set(ASPS_CHAT_VERSION_MAJOR     1)
set(ASPS_CHAT_VERSION_MINOR     0)
set(ASPS_CHAT_VERSION_PATCH     0)
set(ASPS_CHAT_VERSION_TEXT      "${ASPS_CHAT_VERSION_MAJOR}.${ASPS_CHAT_VERSION_MINOR}.${ASPS_CHAT_VERSION_PATCH}")
set(ASPS_CHAT_VERSION_TEXT_DEF  "${ASPS_CHAT_VERSION_MAJOR}.${ASPS_CHAT_VERSION_MINOR}")
set(ASPS_CHAT_SOVERSION_TEXT    "${ASPS_CHAT_VERSION_MAJOR}.${ASPS_CHAT_VERSION_MINOR}")

##################################
# Set CHAT config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/chat_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/chat_config.h
)

##################################
# Set sources for CHAT
set(ASPS_CHAT_SOURCES
    client.cc
    server.cc
    session/session.cc
    msg/chat_seq.cc
    msg/chat_msg.cc
)

##################################
# Build shared library
add_library(aschat SHARED ${ASPS_CHAT_SOURCES})
set_target_properties(aschat
    PROPERTIES
    VERSION     ${ASPS_CHAT_VERSION_TEXT}
    SOVERSION   ${ASPS_CHAT_SOVERSION_TEXT}
)
target_link_libraries(aschat Boost::system pthread)

##################################
# Install library
#install(aschat  LIBRARY DESTINATION lib)
