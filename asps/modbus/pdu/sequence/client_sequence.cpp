// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// All Modbus PDU Sequence.

#include <asps/modbus/pdu/sequence/client_sequence.hpp>

namespace asps {
namespace modbus {
namespace pdu {

// Read Coils Sequence
void client_read_coils_sequence::sync_response()
{
  if (response_->excep_code() == exception_code_success) {
    const read_coils_request* req =
      dynamic_cast<const read_coils_request*>(req_.get());

    if (req) {
      mb_datas& rsp_coils = response_->datas();
      rsp_coils.fill_address(req->address());
      rsp_coils.remove_fill_data(req->quantity());
    }
  }
}

// Read Discrete Inputs Sequence
void client_read_discrete_inputs_sequence::sync_response()
{
  if (response_->excep_code() == exception_code_success) {
    const read_discrete_inputs_request* req =
      dynamic_cast<const read_discrete_inputs_request*>(req_.get());

    if (req) {
      mb_datas& rsp_inputs = response_->datas();
      rsp_inputs.fill_address(req->address());
      rsp_inputs.remove_fill_data(req->quantity());
    }
  }
}

} // pdu
} // modbus
} // asps
