// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Sequence.

#include <gtest/gtest.h>
#include <asps/modbus/modbus.h>
#include <test/modbus/api/event_test.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using ::testing::_;
using ::testing::Return;

class tcp_adu_client_sequence_test : public ::testing::Test
{
protected:
  tcp_adu_client_sequence_test()
    : c_("127.0.0.1"),
      ce_(c_)
  {
    cs_ = std::make_shared<coils>(100, 10, status_);
  }

  client c_;
  client_event_mock ce_;
  coils::pointer_type cs_;
  bool status_[10];
};

// TEST tcp_adu_client_sequence
TEST_F(tcp_adu_client_sequence_test, get_request_read_coils)
{
  tcp_adu_client_sequence seq(100, 10, &ce_);
  tcp_adu::pointer_type a = seq.get_request(cs_, read_coils);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 100);
  EXPECT_EQ(a->unit_identifier(), 10);
  read_coils_request* req = dynamic_cast<read_coils_request*>(a->pdu().get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->starting_address(), 100);
  EXPECT_EQ(req->quantity_of_coils(), 10);
}

TEST_F(tcp_adu_client_sequence_test, get_request_write_single_coil)
{
  tcp_adu_client_sequence seq(100, 10, &ce_);
  tcp_adu::pointer_type a = seq.get_request(cs_, write_single_coil);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 100);
  EXPECT_EQ(a->unit_identifier(), 10);
  write_single_coil_request* req = dynamic_cast<write_single_coil_request*>(a->pdu().get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->output_address(), 100);
  EXPECT_FALSE(req->output_value());
}

TEST_F(tcp_adu_client_sequence_test, get_request_write_multiple_coils)
{
  tcp_adu_client_sequence seq(100, 10, &ce_);
  tcp_adu::pointer_type a = seq.get_request(cs_, write_multiple_coils);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 100);
  EXPECT_EQ(a->unit_identifier(), 10);
  write_multiple_coils_request* req = dynamic_cast<write_multiple_coils_request*>(a->pdu().get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->starting_address(), 100);
  EXPECT_EQ(req->quantity_of_outputs(), 10);
}

TEST_F(tcp_adu_client_sequence_test, get_request_invalid_excep)
{
  tcp_adu_client_sequence seq(100, 10, &ce_);
  tcp_adu::pointer_type a = seq.get_request(cs_, read_discrete_inputs);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 100);
  EXPECT_EQ(a->unit_identifier(), 10);
  excep_pdu* e = dynamic_cast<excep_pdu*>(a->pdu().get());
  EXPECT_NE(e, nullptr);
}

// Test tcp adu server sequence
class tcp_adu_server_sequence_test : public ::testing::Test
{
protected:
  tcp_adu_server_sequence_test()
    : s_(9502),
      se_(s_)
  {}

  server s_;
  server_event_mock se_;
};

TEST_F(tcp_adu_server_sequence_test, set_request_read_coils)
{
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_read_coils(_))
    .WillOnce(Return(cs));

  tcp_adu_server_sequence seq(&se_);
  mb_pdu::pointer_type pdu_req = std::make_shared<read_coils_request>(100, 10);
  tcp_adu::pointer_type req = std::make_shared<tcp_adu>(100, 1, pdu_req);
  tcp_adu::pointer_type rsp = seq.set_request(req);
  EXPECT_NE(rsp, nullptr);
  mb_pdu::pointer_type pdu_rsp = rsp->pdu();
  EXPECT_NE(pdu_rsp, nullptr);
  read_coils_response* r = dynamic_cast<read_coils_response*>(pdu_rsp.get());
  EXPECT_NE(r, nullptr);
}

TEST_F(tcp_adu_server_sequence_test, set_request_write_single_coil)
{
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_write_single_coil(_))
    .WillOnce(Return(cs));

  tcp_adu_server_sequence seq(&se_);
  mb_pdu::pointer_type pdu_req = std::make_shared<write_single_coil_request>(100, false);
  tcp_adu::pointer_type req = std::make_shared<tcp_adu>(100, 1, pdu_req);
  tcp_adu::pointer_type rsp = seq.set_request(req);
  EXPECT_NE(rsp, nullptr);
  mb_pdu::pointer_type pdu_rsp = rsp->pdu();
  EXPECT_NE(pdu_rsp, nullptr);
  write_single_coil_response* r = dynamic_cast<write_single_coil_response*>(pdu_rsp.get());
  EXPECT_NE(r, nullptr);
}

TEST_F(tcp_adu_server_sequence_test, set_request_write_multiple_coils)
{
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_write_multiple_coils(_))
    .WillOnce(Return(cs));

  tcp_adu_server_sequence seq(&se_);
  bits_type output_values(10);
  mb_pdu::pointer_type pdu_req = std::make_shared<write_multiple_coils_request>(100, 10, output_values);
  tcp_adu::pointer_type req = std::make_shared<tcp_adu>(100, 1, pdu_req);
  tcp_adu::pointer_type rsp = seq.set_request(req);
  EXPECT_NE(rsp, nullptr);
  mb_pdu::pointer_type pdu_rsp = rsp->pdu();
  EXPECT_NE(pdu_rsp, nullptr);
  write_multiple_coils_response* r = dynamic_cast<write_multiple_coils_response*>(pdu_rsp.get());
  EXPECT_NE(r, nullptr);
}

} // modbus_test
} // asps_test
