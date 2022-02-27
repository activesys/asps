// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus Global Event.

#include <asps/modbus/common/global_event.h>

using namespace asps::modbus;

// Global Client Event
global_client_event* global_client_event::instance_ = nullptr;

// Global Server Event
global_server_event* global_server_event::instance_ = nullptr;
