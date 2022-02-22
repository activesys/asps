// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Unit test for Modbus PDU Message.

#include <gtest/gtest.h>
#include <asps/modbus/modbus.h>

namespace asps_test {
namespace modbus_test {

using namespace asps::modbus;

// Test PDU unserialize
TEST(pdu_test, unserialize_invald_pdu)
{
  uint8_t msg[2] = {0x7a, 0x04};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, true);
  excep_pdu* excep = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(excep, nullptr);
  EXPECT_EQ(excep->code(), server_device_failure);
  EXPECT_EQ(excep->function_code(), 0xe4);
}

TEST(pdu_test, unserialize_excep_pdu)
{
  uint8_t msg[2] = {0x81, 0x04};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, true);
  excep_pdu* excep = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(excep, nullptr);
}

TEST(pdu_test, unserialize_read_coils_request)
{
  uint8_t msg[5] = {0x01, 0x01, 0x01, 0x3, 0xf2};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, true);
  read_coils_request* req = dynamic_cast<read_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->function_code(), read_coils);
  EXPECT_EQ(req->starting_address(), 257);
  EXPECT_EQ(req->quantity_of_coils(), 1010);
}

TEST(pdu_test, unserialize_read_coils_response)
{
  uint8_t msg[5] = {0x01, 0x03, 0xcd, 0x6b, 0x05};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, false);
  read_coils_response* rsp = dynamic_cast<read_coils_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
  mb_pdu::coils_type cs = rsp->status();
  EXPECT_EQ(cs.size(), 24); 
}

TEST(pdu_test, unserialize_write_single_coil_request)
{
  uint8_t msg[5] = {0x05, 0x00, 0x64, 0xff, 0x00};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, true);
  write_single_coil_request* req =
    dynamic_cast<write_single_coil_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST(pdu_test, unserialize_write_multiple_coils_request)
{
  uint8_t msg[8] = {0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, true);
  write_multiple_coils_request* req = dynamic_cast<write_multiple_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
}

TEST(pdu_test, unserialize_write_multiple_coils_response)
{
  uint8_t msg[5] = {0x0f, 0x00, 0x64, 0x00, 0x64};
  mb_pdu::pointer_type p = mb_pdu::unserialize(msg, false);
  write_multiple_coils_response* rsp = dynamic_cast<write_multiple_coils_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
}

// Test excep_pdu
TEST(excep_pdu_test, serialized_size)
{
  excep_pdu excep(read_coils, illegal_data_address);
  EXPECT_EQ(excep.serialized_size(), 2);
}

TEST(excep_pdu_test, serialize)
{
  excep_pdu excep(read_coils, illegal_data_address);
  uint8_t* msg = excep.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x81);
  EXPECT_EQ(msg[1], 0x02);
}

TEST(excep_pdu_test, unserialize)
{
  uint8_t msg[2] = {0x81, 0x04};
  mb_pdu::pointer_type p = excep_pdu::unserialize(msg);
  excep_pdu* excep = dynamic_cast<excep_pdu*>(p.get());
  EXPECT_NE(excep, nullptr);
  EXPECT_EQ(excep->code(), server_device_failure);
  EXPECT_EQ(excep->function_code(), 0x81);
}

// Test read_coils_request
TEST(read_coils_request, serialized_size)
{
  read_coils_request req(19, 100);
  EXPECT_EQ(req.serialized_size(), 5);
}

TEST(read_coils_request, serialize)
{
  read_coils_request req(19, 100);
  uint8_t* msg = req.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], read_coils);
  EXPECT_EQ(msg[1], 0x00);
  EXPECT_EQ(msg[2], 0x13);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x64);
}

TEST(read_coils_request, unserialize)
{
  uint8_t msg[5] = {0x01, 0x01, 0x01, 0x3, 0xf2};
  mb_pdu::pointer_type p = read_coils_request::unserialize(msg);
  read_coils_request* req = dynamic_cast<read_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->function_code(), read_coils);
  EXPECT_EQ(req->starting_address(), 257);
  EXPECT_EQ(req->quantity_of_coils(), 1010);
}

// Test read_coils_response
TEST(read_coils_response, serialized_size)
{
  mb_pdu::coils_type cs(100, true);
  read_coils_response rsp(cs);
  EXPECT_EQ(rsp.serialized_size(), 15);
}

TEST(read_coils_response, serialize)
{
  mb_pdu::coils_type cs(100, true);
  cs[32] = false;
  cs[45] = false;
  cs[47] = false;
  cs[88] = false;
  cs[90] = false;
  cs[92] = false;
  read_coils_response rsp(cs);
  uint8_t* msg = rsp.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x01);
  EXPECT_EQ(msg[1], 13);
  EXPECT_EQ(msg[2], 0xff);// 7~0: 1111 1111
  EXPECT_EQ(msg[3], 0xff);// 15~8: 1111 1111
  EXPECT_EQ(msg[4], 0xff);// 23~16: 1111 1111
  EXPECT_EQ(msg[5], 0xff);// 31~24: 1111 1111
  EXPECT_EQ(msg[6], 0xfe);// 39~32: 1111 1110
  EXPECT_EQ(msg[7], 0x5f);// 47~40: 0101 1111
  EXPECT_EQ(msg[8], 0xff);// 55~48: 1111 1111
  EXPECT_EQ(msg[9], 0xff);// 63~56: 1111 1111
  EXPECT_EQ(msg[10], 0xff);// 71~64: 1111 1111
  EXPECT_EQ(msg[11], 0xff);// 79~72: 1111 1111
  EXPECT_EQ(msg[12], 0xff);// 87~80: 1111 1111
  EXPECT_EQ(msg[13], 0xea);// 95~88: 1110 1010
  EXPECT_EQ(msg[14], 0x0f);// 99~96: 0000 1111
}

TEST(read_coils_response, unserialize)
{
  uint8_t msg[5] = {0x01, 0x03, 0xcd, 0x6b, 0x05};
  mb_pdu::pointer_type p = read_coils_response::unserialize(msg);
  read_coils_response* rsp = dynamic_cast<read_coils_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
  mb_pdu::coils_type cs = rsp->status();
  EXPECT_EQ(cs.size(), 24); 
  // 7~0 1100 1101
  // 15~8 0110 1011
  // 23~16 0000 0101
  EXPECT_TRUE(cs[0]);
  EXPECT_FALSE(cs[1]);
  EXPECT_TRUE(cs[2]);
  EXPECT_TRUE(cs[3]);
  EXPECT_FALSE(cs[4]);
  EXPECT_FALSE(cs[5]);
  EXPECT_TRUE(cs[6]);
  EXPECT_TRUE(cs[7]);
  EXPECT_TRUE(cs[8]);
  EXPECT_TRUE(cs[9]);
  EXPECT_FALSE(cs[10]);
  EXPECT_TRUE(cs[11]);
  EXPECT_FALSE(cs[12]);
  EXPECT_TRUE(cs[13]);
  EXPECT_TRUE(cs[14]);
  EXPECT_FALSE(cs[15]);
  EXPECT_TRUE(cs[16]);
  EXPECT_FALSE(cs[17]);
  EXPECT_TRUE(cs[18]);
  EXPECT_FALSE(cs[19]);
  EXPECT_FALSE(cs[20]);
  EXPECT_FALSE(cs[21]);
  EXPECT_FALSE(cs[22]);
  EXPECT_FALSE(cs[23]);
}

// TEST write_single_coil_request
TEST(write_single_coil_request, serialized_size)
{
  write_single_coil_request req(100, false);
  EXPECT_EQ(req.serialized_size(), 5);
}

TEST(write_single_coil_request, serialize)
{
  write_single_coil_request req(100, true);
  uint8_t* msg = req.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], write_single_coil);
  EXPECT_EQ(msg[1], 0x00);
  EXPECT_EQ(msg[2], 0x64);
  EXPECT_EQ(msg[3], 0xff);
  EXPECT_EQ(msg[4], 0x00);
}

TEST(write_single_coil_request, unserialize)
{
  uint8_t msg[5] = {0x05, 0x00, 0x64, 0xff, 0x00};
  mb_pdu::pointer_type p = write_single_coil_request::unserialize(msg);
  write_single_coil_request* req =
    dynamic_cast<write_single_coil_request*>(p.get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->output_address(), 100);
  EXPECT_TRUE(req->output_value());
}

// TEST write_multiple_coils_request
TEST(write_multiple_coils_request, serialized_size)
{
  mb_pdu::coils_type cs(100);
  write_multiple_coils_request req(100, 100, cs);
  EXPECT_EQ(req.serialized_size(), 19);
}

TEST(write_multiple_coils_request, serialize)
{
  mb_pdu::coils_type cs(10);
  // 7~0 1100 1101
  // 9~8 0000 0001
  cs[0] = true;
  cs[2] = true;
  cs[3] = true;
  cs[6] = true;
  cs[7] = true;
  cs[8] = true;
  write_multiple_coils_request req(19, 10, cs);
  uint8_t* msg = req.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x0f);
  EXPECT_EQ(msg[1], 0x00);
  EXPECT_EQ(msg[2], 0x13);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x0a);
  EXPECT_EQ(msg[5], 0x02);
  EXPECT_EQ(msg[6], 0xcd);
  EXPECT_EQ(msg[7], 0x01);
}

TEST(write_multiple_coils_request, unserialize)
{
  uint8_t msg[8] = {0x0f, 0x00, 0x13, 0x00, 0x0a, 0x02, 0xcd, 0x01};
  mb_pdu::pointer_type p = write_multiple_coils_request::unserialize(msg);
  write_multiple_coils_request* req = dynamic_cast<write_multiple_coils_request*>(p.get());
  EXPECT_NE(req, nullptr);
  EXPECT_EQ(req->starting_address(), 19);
  EXPECT_EQ(req->quantity_of_outputs(), 10);
  mb_pdu::coils_type cs = req->outputs_value();
  // 7~0 1100 1101
  // 15~8 0000 0001
  EXPECT_TRUE(cs[0]);
  EXPECT_FALSE(cs[1]);
  EXPECT_TRUE(cs[2]);
  EXPECT_TRUE(cs[3]);
  EXPECT_FALSE(cs[4]);
  EXPECT_FALSE(cs[5]);
  EXPECT_TRUE(cs[6]);
  EXPECT_TRUE(cs[7]);
  EXPECT_TRUE(cs[8]);
  EXPECT_FALSE(cs[9]);
  EXPECT_FALSE(cs[10]);
  EXPECT_FALSE(cs[11]);
  EXPECT_FALSE(cs[12]);
  EXPECT_FALSE(cs[13]);
  EXPECT_FALSE(cs[14]);
}

// TEST write_multiple_coils_response
TEST(write_multiple_coils_response, serialized_size)
{
  write_multiple_coils_response rsp(100, 100);
  EXPECT_EQ(rsp.serialized_size(), 5);
}

TEST(write_multiple_coils_response, serialize)
{
  write_multiple_coils_response rsp(100, 100);
  uint8_t* msg = rsp.serialize();
  EXPECT_NE(msg, nullptr);
  EXPECT_EQ(msg[0], 0x0f);
  EXPECT_EQ(msg[1], 0x00);
  EXPECT_EQ(msg[2], 0x64);
  EXPECT_EQ(msg[3], 0x00);
  EXPECT_EQ(msg[4], 0x64);
}

TEST(write_multiple_coils_response, unserialize)
{
  uint8_t msg[5] = {0x0f, 0x00, 0x64, 0x00, 0x64};
  mb_pdu::pointer_type p = write_multiple_coils_response::unserialize(msg);
  write_multiple_coils_response* rsp = dynamic_cast<write_multiple_coils_response*>(p.get());
  EXPECT_NE(rsp, nullptr);
  EXPECT_EQ(rsp->starting_address(), 100);
  EXPECT_EQ(rsp->quantity_of_outputs(), 100);
}

} // modbus_test
} // asps_test
