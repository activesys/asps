// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Sequence.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using ::testing::_;

class client_event_mock : public client_event
{
public:
  client_event_mock(client& c)
    : client_event(c)
  {}

public:
  MOCK_METHOD(void, on_connect, (const std::string& address, uint16_t port), (override));
  MOCK_METHOD(void, on_connect, (const std::string& error_message), (override));
  MOCK_METHOD(void, on_error, (const std::string& error_message), (override));
  MOCK_METHOD(void, on_read_coils, (const coils::ptr_type cs, exception_code code), (override));
  MOCK_METHOD(void, on_write_single_coil, (const coils::ptr_type cs, exception_code code), (override));
  MOCK_METHOD(void, on_write_multiple_coils, (const coils::ptr_type cs, exception_code code), (override));
};

class pdu_sequence_test : public ::testing::Test
{
protected:
  pdu_sequence_test()
    : c_("127.0.0.1"),
      ce_(c_)
  {
    cs_ = std::make_shared<coils>(100, 10, status_);
  }

  client c_;
  client_event_mock ce_;
  coils::ptr_type cs_;
  bool status_[10];
};

// TEST read_coils_pdu_client_sequence
TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_get_request)
{
  read_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = seq.get_request();
  read_coils_request* req = dynamic_cast<read_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_response)
{
  read_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu::coils_type ct(10);
  pdu_ptr p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_read_coils(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_exception)
{
  read_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<excep_pdu>(pdu::read_coils, illegal_function);
  EXPECT_CALL(ce_, on_read_coils(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_error)
{
  read_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<write_single_coil_request>(100, false);
  EXPECT_CALL(ce_, on_error("Invalid Read Coils Response PDU"))
    .Times(1);
  seq.set_response(p);
}

// TEST write_single_coil_pdu_client_sequence
TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_get_request)
{
  write_single_coil_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = seq.get_request();
  write_single_coil_request* req = dynamic_cast<write_single_coil_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response)
{
  write_single_coil_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<write_single_coil_response>(100, false);
  EXPECT_CALL(ce_, on_write_single_coil(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response_exception)
{
  write_single_coil_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<excep_pdu>(pdu::write_single_coil, illegal_function);
  EXPECT_CALL(ce_, on_write_single_coil(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response_error)
{
  write_single_coil_pdu_client_sequence seq(cs_, &ce_);
  pdu::coils_type ct(10);
  pdu_ptr p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_error("Invalid Write Single Coil Response PDU"))
    .Times(1);
  seq.set_response(p);
}

// TEST write_multiple_coils_pdu_client_sequence
TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_get_request)
{
  write_multiple_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = seq.get_request();
  write_multiple_coils_request* req =
    dynamic_cast<write_multiple_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response)
{
  write_multiple_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<write_multiple_coils_response>(100, 10);
  EXPECT_CALL(ce_, on_write_multiple_coils(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response_exception)
{
  write_multiple_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu_ptr p = std::make_shared<excep_pdu>(pdu::write_multiple_coils, illegal_function);
  EXPECT_CALL(ce_, on_write_multiple_coils(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response_exception_error)
{
  write_multiple_coils_pdu_client_sequence seq(cs_, &ce_);
  pdu::coils_type ct(10);
  pdu_ptr p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_error("Invalid Write Multiple Coils Response PDU"))
    .Times(1);
  seq.set_response(p);
}

} // modbus_test
} // asps_test
