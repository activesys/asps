# Copyright (c) 2021 The asps Authors. All rights reserved.
# Use of this source code is governed by a MIT license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

cmake_minimum_required(VERSION 3.10)
project(asps VERSION 1.0.0 LANGUAGES C CXX)
set(ASPS_VERSION           0x010000)
set(ASPS_VERSION_MAJOR     1)
set(ASPS_VERSION_MINOR     0)
set(ASPS_VERSION_PATCH     0)

###############################
# This project requires C/C++11.
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED ON)
  set(CMAKE_C_EXTENSIONS OFF)
endif(NOT CMAKE_C_STANDARD)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

###############################
# Add config file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/asps_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/asps_config.h
)

##################################
# Set include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

###############################
# Checking include files
include(CheckIncludeFile)
check_include_file("stdint.h" HAVE_STDINT_H)

##################################
# Checking third-party libraries

##################################
# Build libraries
add_subdirectory(asps)

##################################
# Build testing
find_package(PkgConfig)
pkg_search_module(GTEST gtest_main)
if (GTEST_FOUND)
  add_subdirectory(test)
endif (GTEST_FOUND)
