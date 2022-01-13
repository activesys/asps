# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

##################################
# Build chat demos executable
add_executable(chat_client chat_client.cc)
target_link_libraries(chat_client aschat)

add_executable(chat_server chat_server.cc)
target_link_libraries(chat_server aschat)

