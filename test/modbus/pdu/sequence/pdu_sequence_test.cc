// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Sequence.

#include <gtest/gtest.h>
#include <asps/modbus/modbus.h>
#include <test/modbus/api/event_test.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;
using ::testing::_;
using ::testing::Return;

class transport_layer_test : public transport_layer
{
public:
  void connect(connect_handler on_connect,
               error_handler on_error) {};
  void write(const uint8_t* buffer,
             std::size_t length,
             eof_handler on_eof,
             error_handler on_error) {};
  void read(std::size_t length,
            read_handler on_read,
            eof_handler on_eof,
            error_handler on_error) {};
  void glance(std::size_t length,
              glance_handler on_glance,
              eof_handler on_eof,
              error_handler on_error) {};
};

class pdu_sequence_test : public ::testing::Test
{
protected:
  pdu_sequence_test()
    : c_(1, layer_),
      ce_(c_)
  {
    cs_ = std::make_shared<coils>(100, 10, status_);
    global_client_event::instance()->event(&ce_);
  }

  transport_layer_test layer_;
  client c_;
  client_event_mock ce_;
  coils::pointer_type cs_;
  bool status_[10];
};

// TEST read_coils_pdu_client_sequence
TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_get_request)
{
  read_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type p = seq.get_request();
  read_coils_request* req = dynamic_cast<read_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_response)
{
  read_coils_pdu_client_sequence seq(cs_);
  bits_type ct(10);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_read_coils(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_exception)
{
  read_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<excep_pdu>(read_coils, illegal_function);
  EXPECT_CALL(ce_, on_read_coils(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, read_coils_pdu_client_sequence_set_response_error)
{
  read_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<write_single_coil_request>(100, false);
  EXPECT_CALL(ce_, on_error("Invalid Read Coils Response PDU"))
    .Times(1);
  seq.set_response(p);
}

// TEST write_single_coil_pdu_client_sequence
TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_get_request)
{
  write_single_coil_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type p = seq.get_request();
  write_single_coil_request* req = dynamic_cast<write_single_coil_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response)
{
  write_single_coil_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<write_single_coil_response>(100, false);
  EXPECT_CALL(ce_, on_write_single_coil(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response_exception)
{
  write_single_coil_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<excep_pdu>(write_single_coil, illegal_function);
  EXPECT_CALL(ce_, on_write_single_coil(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_single_coil_pdu_client_sequence_set_response_error)
{
  write_single_coil_pdu_client_sequence seq(cs_);
  bits_type ct(10);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_error("Invalid Write Single Coil Response PDU"))
    .Times(1);
  seq.set_response(p);
}

// TEST write_multiple_coils_pdu_client_sequence
TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_get_request)
{
  write_multiple_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type p = seq.get_request();
  write_multiple_coils_request* req =
    dynamic_cast<write_multiple_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response)
{
  write_multiple_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<write_multiple_coils_response>(100, 10);
  EXPECT_CALL(ce_, on_write_multiple_coils(_, success))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response_exception)
{
  write_multiple_coils_pdu_client_sequence seq(cs_);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<excep_pdu>(write_multiple_coils, illegal_function);
  EXPECT_CALL(ce_, on_write_multiple_coils(_, illegal_function))
    .Times(1);
  seq.set_response(p);
}

TEST_F(pdu_sequence_test, write_multiple_coils_pdu_client_sequence_set_response_exception_error)
{
  write_multiple_coils_pdu_client_sequence seq(cs_);
  bits_type ct(10);
  mb_pdu::pointer_type req = seq.get_request();
  mb_pdu::pointer_type p = std::make_shared<read_coils_response>(ct);
  EXPECT_CALL(ce_, on_error("Invalid Write Multiple Coils Response PDU"))
    .Times(1);
  seq.set_response(p);
}

// Test pdu server sequence
class pdu_server_sequence_test : public ::testing::Test
{
protected:
  pdu_server_sequence_test()
    : s_(9502),
      se_(s_)
  {
    global_server_event::instance()->event(&se_);
  }

  server s_;
  server_event_mock se_;
};

TEST_F(pdu_server_sequence_test, read_coils_pdu_server_sequence_set_request_success)
{
  read_coils_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_read_coils(_))
    .WillOnce(Return(cs));

  mb_pdu::pointer_type req = std::make_shared<read_coils_request>(100, 10);
  mb_pdu::pointer_type q = seq.set_request(req);
  read_coils_response* rsp = dynamic_cast<read_coils_response*>(q.get());
  EXPECT_NE(rsp, nullptr);
}

TEST_F(pdu_server_sequence_test, read_coils_pdu_server_sequence_set_request_failed)
{
  read_coils_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  cs->code(illegal_data_address);
  EXPECT_CALL(se_, on_read_coils(_))
    .WillOnce(Return(cs));

  mb_pdu::pointer_type req = std::make_shared<read_coils_request>(100, 10);
  mb_pdu::pointer_type q = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(q.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x81);
  EXPECT_EQ(e->code(), illegal_data_address);
}

TEST_F(pdu_server_sequence_test, read_coils_pdu_server_sequence_set_request_invalid_request)
{
  read_coils_pdu_server_sequence seq;
  EXPECT_CALL(se_, on_error("Invalid Read Coils Request PDU"))
    .Times(1);

  mb_pdu::pointer_type req = std::make_shared<write_single_coil_request>(100, false);
  mb_pdu::pointer_type q = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(q.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x81);
  EXPECT_EQ(e->code(), server_device_failure);
}

TEST_F(pdu_server_sequence_test, read_coils_pdu_server_sequence_set_request_null_event)
{
  global_server_event::instance()->event(nullptr);
  read_coils_pdu_server_sequence seq;
  mb_pdu::pointer_type req = std::make_shared<read_coils_request>(100, false);
  mb_pdu::pointer_type q = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(q.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x81);
  EXPECT_EQ(e->code(), server_device_failure);
}

TEST_F(pdu_server_sequence_test, write_single_coil_pdu_server_sequence_set_request_success)
{
  global_server_event::instance()->event(&se_);
  write_single_coil_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_write_single_coil(_))
    .WillOnce(Return(cs));

  mb_pdu::pointer_type req = std::make_shared<write_single_coil_request>(100, true);
  mb_pdu::pointer_type p = seq.set_request(req);
  write_single_coil_response* rsp = dynamic_cast<write_single_coil_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
}

TEST_F(pdu_server_sequence_test, write_single_coil_pdu_server_sequence_set_request_failed)
{
  write_single_coil_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  cs->code(illegal_data_address);
  EXPECT_CALL(se_, on_write_single_coil(_))
    .WillOnce(Return(cs));

  mb_pdu::pointer_type req = std::make_shared<write_single_coil_request>(100, true);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x85);
  EXPECT_EQ(e->code(), illegal_data_address);
}

TEST_F(pdu_server_sequence_test, write_single_coil_pdu_server_sequence_set_request_invalid_request)
{
  write_single_coil_pdu_server_sequence seq;
  EXPECT_CALL(se_, on_error("Invalid Write Single Coil Request PDU"))
    .Times(1);

  mb_pdu::pointer_type req = std::make_shared<read_coils_request>(100, 10);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x85);
  EXPECT_EQ(e->code(), server_device_failure);
}

TEST_F(pdu_server_sequence_test, write_single_coil_pdu_server_sequence_set_request_null_event)
{
  global_server_event::instance()->event(nullptr);
  write_single_coil_pdu_server_sequence seq;
  mb_pdu::pointer_type req = std::make_shared<write_single_coil_request>(100, true);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x85);
  EXPECT_EQ(e->code(), server_device_failure);
}

TEST_F(pdu_server_sequence_test, write_multiple_coils_pdu_server_sequence_set_request_success)
{
  global_server_event::instance()->event(&se_);
  write_multiple_coils_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  EXPECT_CALL(se_, on_write_multiple_coils(_))
    .WillOnce(Return(cs));

  bits_type output_values(10);
  mb_pdu::pointer_type req = std::make_shared<write_multiple_coils_request>(100, 10, output_values);
  mb_pdu::pointer_type p = seq.set_request(req);
  write_multiple_coils_response* rsp = dynamic_cast<write_multiple_coils_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
}

TEST_F(pdu_server_sequence_test, write_multiple_coils_pdu_server_sequence_set_request_failed)
{
  write_multiple_coils_pdu_server_sequence seq;
  bool status[10];
  coils::pointer_type cs = std::make_shared<coils>(100, 10, status);
  cs->code(illegal_data_address);
  EXPECT_CALL(se_, on_write_multiple_coils(_))
    .WillOnce(Return(cs));

  bits_type output_values(10);
  mb_pdu::pointer_type req = std::make_shared<write_multiple_coils_request>(100, 10, output_values);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x8f);
  EXPECT_EQ(e->code(), illegal_data_address);
}

TEST_F(pdu_server_sequence_test, write_multiple_coils_pdu_server_sequence_set_request_invalid_request)
{
  write_multiple_coils_pdu_server_sequence seq;
  EXPECT_CALL(se_, on_error("Invalid Write Multiple Coils Request PDU"))
    .Times(1);

  mb_pdu::pointer_type req = std::make_shared<read_coils_request>(100, 10);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x8f);
  EXPECT_EQ(e->code(), server_device_failure);
}

TEST_F(pdu_server_sequence_test, write_multiple_coils_pdu_server_sequence_set_request_null_event)
{
  global_server_event::instance()->event(nullptr);
  write_multiple_coils_pdu_server_sequence seq;
  bits_type output_values(10);
  mb_pdu::pointer_type req = std::make_shared<write_multiple_coils_request>(100, 10, output_values);
  mb_pdu::pointer_type p = seq.set_request(req);
  excep_pdu* e = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->function_code(), 0x8f);
  EXPECT_EQ(e->code(), server_device_failure);
}

} // modbus_test
} // asps_test
