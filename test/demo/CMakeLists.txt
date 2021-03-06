# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# include CTest
enable_testing()

##################################
# Set sources for modbus
set(ASPS_DEMO_TEST_SOURCES
  semantic/demo_client_test.cc
  semantic/demo_server_test.cc
  message/client_data_test.cc
  message/keepalive_test.cc
  message/server_data_test.cc
  config/config_test.cc
  sequence/client_sequence_test.cc
  sequence/server_sequence_test.cc
  session/server_session_test.cc
)

##################################
# Build test executable
add_executable(asdemo_test ${ASPS_DEMO_TEST_SOURCES})
target_link_libraries(asdemo_test ${GTEST_LDFLAGS} ${GMOCK_LDFLAGS} asdemo Boost::system pthread)
target_compile_options(asdemo_test PUBLIC ${GTEST_CFLAGS} ${GMOCK_CFLAGS})

##################################
# Discover test case
include(GoogleTest)
gtest_discover_tests(asdemo_test)
