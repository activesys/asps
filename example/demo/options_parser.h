// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo client command line option parser.

#ifndef EXAMPLE_DEMO_OPTIONS_PARSER_H
#define EXAMPLE_DEMO_OPTIONS_PARSER_H

#include <iostream>
#include <string>
#include <cstdint>
#include <exception>
#include <boost/program_options.hpp>

namespace asps_example {
namespace demo_example {

namespace po = boost::program_options;

class server_options_parser
{
public:
  server_options_parser(int argc, char* argv[])
    : od_("Demo Server 1.0.0.\nUsage")
  {
    try {
      // Version and Help message options
      po::options_description vhod("Demo server version and help options");
      vhod.add_options()
        ("version,v", "show demo server version.")
        ("help,h", "show help message.");

      // Demo server information options
      po::options_description siod("Demo server information options");
      siod.add_options()
        ("port,p", po::value<uint16_t>(&port_)->default_value(9900), "Demo server port.")
        ("t1", po::value<uint32_t>(&t1_)->default_value(30), "Demo server t1.")
        ("t2", po::value<uint32_t>(&t2_)->default_value(20), "Demo server t2.")
        ("debug,d", "debug raw datas received and sent.");

      // Demo Server options
      od_.add(vhod).add(siod);

      po::store(po::parse_command_line(argc, argv, od_), vm_);
      po::notify(vm_);
    } catch (std::exception& e) {
      error_ = e.what();
    }
  }

public:
  operator bool()
  {
    if (t2_ >= t1_) {
      error_ = "t2 must be less than t1.";
    }

    if (error_.size()) {
      return false;
    }
    return !(vm_.count("help") ||
             vm_.count("version"));
  }

public:
  void usage() const
  {
    if (error_.size()) {
      std::cout << error_ << std::endl;
    }
    std::cout << od_ << std::endl;
  }

  bool debug() const
  {
    return vm_.count("debug");
  }

  uint16_t port() const
  {
    return port_;
  }

  uint32_t t1() const
  {
    return t1_;
  }

  uint32_t t2() const
  {
    return t2_;
  }

private:
  po::options_description od_;
  po::variables_map vm_;
  uint16_t port_;
  std::string error_;
  uint32_t t1_;
  uint32_t t2_;
};

class client_options_parser
{
public:
  client_options_parser(int argc, char* argv[])
    : od_("Demo Client 1.0.0.\nUsage")
  {
    try {
      // Version and Help message options
      po::options_description vhod("Demo clien version and help options");
      vhod.add_options()
        ("version,v", "show demo client version.")
        ("help,h", "show help message.");

      // Demo client information options
      po::options_description ciod("Demo client information options");
      ciod.add_options()
        ("ip,i", po::value<std::string>()->required(), "Demo server ip.")
        ("port,p", po::value<uint16_t>()->required(), "Demo server port.")
        ("t0", po::value<uint32_t>(&t0_)->default_value(30), "Demo client t0 in second.")
        ("t1", po::value<uint32_t>(&t1_)->default_value(30), "Demo client t1 in second.")
        ("t2", po::value<uint32_t>(&t2_)->default_value(20), "Demo client t2 in second.")
        ("debug,d", "debug raw datas received and sent.");

      // Demo client data send options
      po::options_description dsod("Demo client data send options");
      dsod.add_options()
        ("frequency,f",
         po::value<uint32_t>()->default_value(1000),
         "Data transmission frequency, in milliseconds.")
        ("times,t",
         po::value<uint32_t>()->default_value(0),
         "Number of times data has been sent.\n"
         "0 indicates an infinite number of times data has been sent.")
        ("dst", "disable the same type of compression options.")
        ("dks", "disable the key sequence of compression options.")
        ("dss", "disable the same timestamp of compression options.")
        ("random,r", "generate random number.")
        ("begin,b", po::value<uint32_t>(&begin_)->default_value(0), "key begin.")
        ("end,e", po::value<uint32_t>(&end_)->default_value(100), "key end.")
        ("type,T", po::value<uint16_t>(&type_)->default_value(6),
         "value type, default type is uint32_t.\n"
         "the specific types are as follows:\n"
         "\t0 is boolean.\n"
         "\t1 is int8_t.\n"
         "\t2 is uint8_t.\n"
         "\t3 is int16_t.\n"
         "\t4 is uint16_t.\n"
         "\t5 is int32_t.\n"
         "\t6 is uint32_t.\n"
         "\t7 is int64_t.\n"
         "\t8 is uint64_t.\n"
         "\t9 is float.\n"
         "\t10 is double.");

      // Demo Client options
      od_.add(vhod).add(ciod).add(dsod);

      po::store(po::parse_command_line(argc, argv, od_), vm_);
      po::notify(vm_);
    } catch (std::exception& e) {
      error_ = e.what();
    }
  }

public:
  operator bool()
  {
    if (error_.size()) {
      return false;
    }

    if (type_ > 10) {
      error_ = "invalid value type.";
      return false;
    }

    if (t2_ >= t1_) {
      error_ = "t2 must be less than t1.";
      return false;
    }

    if (begin_ >= end_) {
      error_ = "the end of key must be larger than the begin.";
      return false;
    }

    if (vm_.count("help") || vm_.count("version")) {
      return false;
    }

    return true;
  }

public:
  void usage() const
  {
    if (error_.size()) {
      std::cout << error_ << std::endl;
    }
    std::cout << od_ << std::endl;
  }

  std::string const& ip() const
  {
    return vm_["ip"].as<std::string>();
  }
  uint16_t port() const
  {
    return vm_["port"].as<uint16_t>();
  }
  uint32_t frequency() const
  {
    return vm_["frequency"].as<uint32_t>();
  }
  uint32_t times() const
  {
    return vm_["times"].as<uint32_t>();
  }
  bool debug() const
  {
    return vm_.count("debug");
  }
  bool disable_same_type() const
  {
    return vm_.count("dst");
  }
  bool disable_key_sequence() const
  {
    return vm_.count("dks");
  }
  bool disbale_same_timestamp() const
  {
    return vm_.count("dss");
  }
  bool random() const
  {
    return vm_.count("random");
  }
  uint32_t begin() const
  {
    return begin_;
  }
  uint32_t end() const
  {
    return end_;
  }
  uint32_t t0() const
  {
    return t0_;
  }
  uint32_t t1() const
  {
    return t1_;
  }
  uint32_t t2() const
  {
    return t2_;
  }
  uint16_t type() const
  {
    return type_;
  }

private:
  po::options_description od_;
  po::variables_map vm_;
  std::string error_;
  uint32_t t1_;
  uint32_t t2_;
  uint32_t t0_;
  uint32_t begin_;
  uint32_t end_;
  uint16_t type_;
};

} // demo_example
} // asps_example

#endif // EXAMPLE_DEMO_OPTIONS_PARSER_H
