// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Message.

#ifndef ASPS_DEMO_MESSAGE_KEEPALIVE_H
#define ASPS_DEMO_MESSAGE_KEEPALIVE_H

#include <cstdint>
#include <cstring>
#include <vector>

#include <asps/demo/config/config.h>
#include <asps/demo/sequence/message_service.h>

namespace asps {
namespace demo {

// Client keepalive and ack
class client_positive_keepalive
  : public message_serialization_service
{
  enum flag_length {
    flag_field_length = 4
  };

public:
  const buffer_type& serialize() override
  {
    buffer_.resize(flag_field_length);
    uint8_t* pos = buffer_.data();

    // flag field is "KEEP"
    memcpy(pos,
           POSITIVE_KEEPALIVE_MESSAGE_FLAG,
           flag_field_length);
    pos += flag_field_length;

    return buffer_;
  }

private:
  buffer_type buffer_;
};

class client_positive_keepalive_ack
  : public message_unserialization_service
{
  enum flag_length {
    flag_field_length = 1
  };

public:
  bool unserialize(buffer_type& buffer) override
  {
    bool ret = !buffer.empty() && buffer[0] == config::pack();
    if (ret) {
      buffer.erase(buffer.begin());
    }
    return ret;
  }
};

class client_negative_keepalive
  : public message_unserialization_service
{
  enum flag_length {
    flag_field_length = 1
  };

public:
  bool unserialize(buffer_type& buffer) override
  {
    bool ret = !buffer.empty() && buffer[0] == config::nkeep();
    if (ret) {
      buffer.erase(buffer.begin());
    }
    return ret;
  }
};

class client_negative_keepalive_ack
  : public message_serialization_service
{
  enum flag_length {
    flag_field_length = 4
  };

public:
  const buffer_type& serialize() override
  {
    buffer_.resize(flag_field_length);
    uint8_t* pos = buffer_.data();

    // flag field is "KACK"
    memcpy(pos,
           NEGATIVE_KEEPALIVE_ACK_MESSAGE_FLAG,
           flag_field_length);
    pos += flag_field_length;

    return buffer_;
  }

private:
  buffer_type buffer_;
};

// Server keepalive and ack
class server_positive_keepalive
  : public message_unserialization_service
{
  enum flag_length {
    flag_field_length = 4
  };

public:
  bool unserialize(buffer_type& buffer) override
  {
    bool ret = !buffer.empty() &&
               memcmp(buffer.data(),
                      POSITIVE_KEEPALIVE_MESSAGE_FLAG,
                      flag_field_length) == 0;
    if (ret) {
      buffer.erase(buffer.begin(), buffer.begin() + flag_field_length);
    }
    return ret;
  }
};

class server_positive_keepalive_ack
  : public message_serialization_service
{
  enum flag_length {
    flag_field_length = 1
  };

public:
  const buffer_type& serialize() override
  {
    buffer_.resize(flag_field_length);
    buffer_[0] = config::pack();

    return buffer_;
  }

private:
  buffer_type buffer_;
};

class server_negative_keepalive
  : public message_serialization_service
{
  enum flag_length {
    flag_field_length = 1
  };

public:
  const buffer_type& serialize() override
  {
    buffer_.resize(flag_field_length);
    buffer_[0] = config::nkeep();

    return buffer_;
  }

private:
  buffer_type buffer_;
};

class server_negative_keepalive_ack
  : public message_unserialization_service
{
  enum flag_length {
    flag_field_length = 4
  };

public:
  bool unserialize(buffer_type& buffer) override
  {
    bool ret = !buffer.empty() &&
               memcmp(buffer.data(),
                      NEGATIVE_KEEPALIVE_ACK_MESSAGE_FLAG,
                      flag_field_length) == 0;
    if (ret) {
      buffer.erase(buffer.begin(), buffer.begin() + flag_field_length);
    }
    return ret;
  }
};

} // demo
} // asps

#endif // ASPS_DEMO_MESSAGE_KEEPALIVE_H
