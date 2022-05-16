// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Modbus client command line option parser.

#ifndef DEMOS_MODBUS_OPTIONS_PARSER_H
#define DEMOS_MODBUS_OPTIONS_PARSER_H

#include <iostream>
#include <cstdint>
#include <string>
#include <boost/program_options.hpp>

namespace asps_demos {
namespace modbus_demos {

namespace po = boost::program_options;

class options_parser
{
public:
  options_parser(int argc, char* argv[])
    : od_("Modbus client 1.0.0.\nUsage")
  {
    // Version and Help message options
    po::options_description vhod("Modbus client version and help options");
    vhod.add_options()
      ("version,v", "show modbus client version.")
      ("help", "show help message.");

    // Modbus server information options
    po::options_description siod("Modbus server information options");
    siod.add_options()
      ("host,h", po::value<std::string>(), "Modbus server address.")
      ("port,p", po::value<int>(&port_)->default_value(502), "Modbus server port.")
      ("unit-identifier,u", po::value<int>(&unit_identifier_)->default_value(1), "Modbus server unit identifier.");

    // Modbus function code and parameters options
    po::options_description fcod("Modbus function code and parameters options");
    fcod.add_options()
      ("function-code,f", po::value<int>(), "Modbus function code:\n"
                                            "  Read Coils(1).\n"
                                            "  Write Single Coil(5).")
      ("starting-address,a", po::value<int>(), "Starting address.")
      ("quantity,q", po::value<int>(), "Quantity of coils or registers.");

    // Operation-related options
    po::options_description orod("Operation-related options");
    orod.add_options()
      ("times,T", po::value<int>(&times_)->default_value(0), "Number of times a command is sent. 0 indicates an infinite number of times.")
      ("frequency,F", po::value<int>(&frequency_)->default_value(1000), "The frequency of the operation, in milliseconds.");

    // Modbus client options
    od_.add(vhod).add(siod).add(fcod).add(orod);

    po::store(po::parse_command_line(argc, argv, od_), vm_);
    po::notify(vm_);
  }

public:
  operator bool() const
  {
    return !(vm_.count("help") ||
             vm_.count("version") ||
             !vm_.count("host") ||
             !vm_.count("function-code"));
  }

public:
  void usage() const
  {
    std::cout << od_ << std::endl;
  }

  int function_code() const
  {
    return vm_["function-code"].as<int>();
  }

  int unit_identifier() const
  {
    return unit_identifier_;
  }

  const std::string& host() const
  {
    return vm_["host"].as<std::string>();
  }

  int port() const
  {
    return port_;
  }

private:
po::options_description od_;
  po::variables_map vm_;
  int port_;
  int unit_identifier_;
  int times_;
  int frequency_;
};

} // modbus_demos
} // asps_demos

#endif // DEMOS_MODBUS_OPTIONS_PARSER_H
