# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# include CTest
enable_testing()

##################################
# Set sources for modbus
set(ASPS_MODBUS_TEST_SOURCES
    pdu/semantic/client_test.cpp
    pdu/semantic/request_test.cpp
    pdu/session/session_test.cpp
    pdu/sequence/client_sequence_test.cpp
    pdu/message/client_message_test.cpp
)

##################################
# Build test executable
add_executable(asmodbus_test ${ASPS_MODBUS_TEST_SOURCES})
target_link_libraries(asmodbus_test ${GTEST_LDFLAGS} ${GMOCK_LDFLAGS} asmodbus)
target_compile_options(asmodbus_test PUBLIC ${GTEST_CFLAGS} ${GMOCK_CFLAGS})

##################################
# Discover test case
include(GoogleTest)
gtest_discover_tests(asmodbus_test)
