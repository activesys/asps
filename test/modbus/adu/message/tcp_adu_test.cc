// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus ADU Message.

#include <gtest/gtest.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// TEST tcp_adu
TEST(tcp_adu_test, serialized_size_excep_pdu)
{
  mb_pdu::pointer_type p = std::make_shared<excep_pdu>(read_coils, illegal_function);
  tcp_adu adu(1, 1, p);
  EXPECT_EQ(adu.serialized_size(), 9);
}

TEST(tcp_adu_test, serialized_size_read_coils_request)
{
  mb_pdu::pointer_type p = std::make_shared<read_coils_request>(100, 10);
  tcp_adu adu(1, 1, p);
  EXPECT_EQ(adu.serialized_size(), 12);
}

TEST(tcp_adu_test, serialized_size_read_coils_response)
{
  bits_type status{1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1};
  mb_pdu::pointer_type p = std::make_shared<read_coils_response>(status);
  tcp_adu adu(2, 1, p);
  EXPECT_EQ(adu.serialized_size(), 12);
}

TEST(tcp_adu_test, serialize_excep_pdu)
{
  mb_pdu::pointer_type p = std::make_shared<excep_pdu>(read_coils, illegal_function);
  tcp_adu adu(8, 1, p);
  uint8_t* msg = adu.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x00);
  EXPECT_EQ(msg[1], 0x08);
  EXPECT_EQ(msg[2], 0x00);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x00);
  EXPECT_EQ(msg[5], 0x03);
  EXPECT_EQ(msg[6], 0x01);
  EXPECT_EQ(msg[7], 0x81);
  EXPECT_EQ(msg[8], 0x01);
}

TEST(tcp_adu_test, serialize_read_coils_response)
{
  bits_type status{1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1};
  mb_pdu::pointer_type p = std::make_shared<read_coils_response>(status);
  tcp_adu adu(8, 1, p);
  uint8_t* msg = adu.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x00);
  EXPECT_EQ(msg[1], 0x08);
  EXPECT_EQ(msg[2], 0x00);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x00);
  EXPECT_EQ(msg[5], 0x06);
  EXPECT_EQ(msg[6], 0x01);
  EXPECT_EQ(msg[7], 0x01);
  EXPECT_EQ(msg[8], 0x03);
  EXPECT_EQ(msg[9], 0xf1);
  EXPECT_EQ(msg[10], 0x91);
  EXPECT_EQ(msg[11], 0x06);
}

TEST(tcp_adu_test, serialize_read_coils_request)
{
  mb_pdu::pointer_type p = std::make_shared<read_coils_request>(100, 10);
  tcp_adu adu(8, 1, p);
  uint8_t* msg = adu.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x00);
  EXPECT_EQ(msg[1], 0x08);
  EXPECT_EQ(msg[2], 0x00);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x00);
  EXPECT_EQ(msg[5], 0x06);
  EXPECT_EQ(msg[6], 0x01);
  EXPECT_EQ(msg[7], 0x01);
  EXPECT_EQ(msg[8], 0x00);
  EXPECT_EQ(msg[9], 0x64);
  EXPECT_EQ(msg[10], 0x00);
  EXPECT_EQ(msg[11], 0x0a);
}

TEST(tcp_adu_test, unserialize_excep_pdu)
{
  uint8_t msg[9]{0x00, 0x80, 0x00, 0x00, 0x00, 0x03, 0x01, 0x81, 0x01};
  tcp_adu::pointer_type a = tcp_adu::unserialize(msg, true);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 128);
  EXPECT_EQ(a->unit_identifier(), 1);
  excep_pdu* e = dynamic_cast<excep_pdu*>(a->pdu().get());
  EXPECT_NE(e, nullptr);
  EXPECT_EQ(e->code(), 1);
}

TEST(tcp_adu_test, unserialize_read_coils_request)
{
  uint8_t msg[12]{0x00, 0x80, 0x00, 0x00, 0x00, 0x06, 0x01, 0x01, 0x00, 0x64, 0x00, 0x0a};
  tcp_adu::pointer_type a = tcp_adu::unserialize(msg, true);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 128);
  EXPECT_EQ(a->unit_identifier(), 1);
  read_coils_request* req = dynamic_cast<read_coils_request*>(a->pdu().get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->starting_address(), 100);
  EXPECT_EQ(req->quantity_of_coils(), 10);
}

TEST(tcp_adu_test, unserialize_write_single_coil_response)
{
  uint8_t msg[12]{0x00, 0x80, 0x00, 0x00, 0x00, 0x06, 0x08, 0x05, 0x00, 0x64, 0xff, 0x00};
  tcp_adu::pointer_type a = tcp_adu::unserialize(msg, false);
  EXPECT_NE(a, nullptr);
  EXPECT_EQ(a->transaction_identifier(), 128);
  EXPECT_EQ(a->unit_identifier(), 8);
  write_single_coil_response* rsp = dynamic_cast<write_single_coil_response*>(a->pdu().get());
  EXPECT_NE(rsp, nullptr);
  EXPECT_EQ(rsp->output_address(), 100);
  EXPECT_TRUE(rsp->output_value());
}

} // modbus_test
} // asps_test
