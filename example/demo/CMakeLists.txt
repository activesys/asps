# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Build demo protocol example executable
#add_executable(demo_client demo_client.cc)
#target_link_libraries(demo_client asdemo Boost::system Boost::program_options pthread)

add_executable(demo_server demo_server.cc)
target_link_libraries(demo_server asdemo Boost::system Boost::program_options pthread)
