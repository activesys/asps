// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo Client.

#include <random>
#include <functional>
#include <iostream>
#include <iomanip>
#include <example/demo/options_parser.h>
#include <asps/demo/demo.h>
#include <sys/time.h>

using namespace asps::demo;
using namespace asps_example::demo_example;

uint64_t make_timestamp()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

class my_demo_client : public demo_client
{
public:
  my_demo_client(const client_options_parser& cop)
    : demo_client(cop.ip(), cop.port()),
      cop_(cop),
      count_(0),
      dre_(std::random_device()())
  {
    timer_ = make_timer(cop_.frequency(),
                        std::bind(&my_demo_client::on_timeout,
                        this));
  }

public:
  void on_timeout()
  {
    // send datas
    data_group_type datas;
    std::cout << std::endl;

    switch (cop_.type()) {
    case demo_data::boolean_type:
      {
        std::bernoulli_distribution uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<bool>(i, cop_.random() ? uid(dre_) : i%2, make_timestamp());
          bool v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << std::boolalpha << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::int8_type:
      {
        std::uniform_int_distribution<int8_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<int8_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          int8_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << int16_t(v)
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::uint8_type:
      {
        std::uniform_int_distribution<uint8_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<uint8_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          uint8_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << uint16_t(v)
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::int16_type:
      {
        std::uniform_int_distribution<int16_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<int16_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          int16_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::uint16_type:
      {
        std::uniform_int_distribution<uint16_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<uint16_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          uint16_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::int32_type:
      {
        std::uniform_int_distribution<int32_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<int32_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          int32_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::uint32_type:
      {
        std::uniform_int_distribution<uint32_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<uint32_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          uint32_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::int64_type:
      {
        std::uniform_int_distribution<int64_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<int64_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          int64_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::uint64_type:
      {
        std::uniform_int_distribution<uint64_t> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<uint64_t>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          uint64_t v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::float32_type:
      {
        std::uniform_real_distribution<float> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<float>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          float v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;

    case demo_data::float64_type:
      {
        std::uniform_real_distribution<double> uid;
        for (uint32_t i = cop_.begin(); i < cop_.end(); ++i) {
          demo_data::pointer_type d =
            make_demo_data<double>(i, cop_.random() ? uid(dre_) : i, make_timestamp());
          double v;
          d->get_value(&v);
          std::cout << "demo_client> key: " << d->key()
                    << " timestamp: " << d->timestamp()
                    << " value: " << v
                    << std::endl;
          datas.push_back(d);
        }
      }
      break;
    }

    send(datas);
  }

public:
  void on_connect(const connection::pointer_type conn) override
  {
    std::cout << std::endl
              << "demo_client> establish a connection to "
              << conn->remote_address() << ":"
              << conn->remote_port() << std::endl;

    timer_->start();
  }
  void on_close(const connection::pointer_type conn) override
  {
    std::cout << std::endl
              << "demo_client> disconnect the connection to "
              << conn->remote_address() << ":"
              << conn->remote_port() << std::endl;

    timer_->stop();
  }
  void on_read_raw(const connection::pointer_type conn,
                   const buffer_type& buffer,
                   std::size_t bytes) override
  {
    if (cop_.debug()) {
      std::cout << std::endl
                << "demo_client> receive raw datas from "
                << conn->remote_address() << ":"
                << conn->remote_port() << std::endl;
      std::cout << "demo_client>";
      for (std::size_t i = 0; i < bytes; ++i) {
        if (i != 0 && (i % 16 == 0)) {
          std::cout << std::endl << "demo_client>";
        }
        std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << uint16_t(buffer[i]);
      }
      std::cout << std::dec << std::endl;
    }
  }
  void on_write_raw(const connection::pointer_type conn,
                   const buffer_type& buffer,
                   std::size_t bytes) override
  {
    if (cop_.debug()) {
      std::cout << std::endl
                << "demo_client> send raw datas to "
                << conn->remote_address() << ":"
                << conn->remote_port() << std::endl;
      std::cout << "demo_client>";
      for (std::size_t i = 0; i < bytes; ++i) {
        if (i != 0 && (i % 16 == 0)) {
          std::cout << std::endl << "demo_client>";
        }
        std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << uint16_t(buffer[i]);
      }
      std::cout << std::dec << std::endl;
    }

    count_++;
    if (cop_.times() != 0 && count_ < cop_.times()) {
      timer_->start();
    } else {
      stop();
    }
  }

private:
  uint32_t count_;
  const client_options_parser& cop_;
  timer_service::pointer_type timer_;
  std::default_random_engine dre_;
};

int main(int argc, char* argv[])
{
  client_options_parser cop(argc, argv);
  if (!cop) {
    cop.usage();
    return 1;
  }

  config::t0(cop.t0());
  config::t1_t2(cop.t1(), cop.t2());
  config::same_type(!cop.disable_same_type());
  config::key_sequence(!cop.disable_key_sequence());
  config::same_timestamp(!cop.disbale_same_timestamp());
  my_demo_client client(cop);
  client.run();

  return 0;
}
