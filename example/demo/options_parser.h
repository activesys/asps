// Copyright (c) 2021 The asps Authors. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Demo client command line option parser.

#ifndef EXAMPLE_DEMO_OPTIONS_PARSER_H
#define EXAMPLE_DEMO_OPTIONS_PARSER_H

#include <iostream>
#include <string>
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
    // Version and Help message options
    po::options_description vhod("Demo server version and help options");
    vhod.add_options()
      ("version,v", "show demo server version.")
      ("help,h", "show help message.");

    // Demo server information options
    po::options_description siod("Demo server information options");
    siod.add_options()
      ("port,p", po::value<int>(&port_)->default_value(9900), "Demo server port.")
      ("debug,d", "debug raw datas received and sent.");

    // Demo Server options
    od_.add(vhod).add(siod);

    po::store(po::command_line_parser(argc, argv).options(od_).allow_unregistered().run(), vm_);
    po::notify(vm_);
  }

public:
  operator bool() const
  {
    return !(vm_.count("help") ||
             vm_.count("version"));
  }

public:
  void usage() const
  {
    std::cout << od_ << std::endl;
  }

  bool debug() const
  {
    return vm_.count("debug");
  }

  int port() const
  {
    return port_;
  }

private:
  po::options_description od_;
  po::variables_map vm_;
  int port_;
};

} // demo_example
} // asps_example

#endif // EXAMPLE_DEMO_OPTIONS_PARSER_H
