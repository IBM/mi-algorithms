# Copyright (C) tkornuta, IBM Corporation 2015-2019
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Project name
project(MIAlgorithms CXX C)

#  Set current version number.
set (MIAlgorithms_MAJOR_VERSION 1)
set (MIAlgorithms_MINOR_VERSION 2)
set (MIAlgorithms_PATCH_VERSION 1)
set (MIAlgorithms_VERSION ${MIAlgorithms_MAJOR_VERSION}.${MIAlgorithms_MINOR_VERSION}.${MIAlgorithms_PATCH_VERSION})

# CMake required version.
cmake_minimum_required(VERSION 2.8)

# =======================================================================
# Set compiler/linker flags.
# =======================================================================

# Add C++11 dependency.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Check, whether all necessary libraries are linked
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ") 


# =======================================================================
# Create CMAKE variables storing preprocessor commands - FOR HTM! DEPRICATED!
# =======================================================================

# Default length of SDR !!DEPRICATED - TO BE REMOVED!!
set(DEFAULT_SDR_LENGTH 1024 CACHE STRING "Default length of SDR")
add_definitions(-DDEFAULT_SDR_LENGTH=${DEFAULT_SDR_LENGTH})

# Debug !!DEPRICATED - TO BE REMOVED!!
set(DEBUG_RUN ON CACHE BOOL "Debug run - set DEBUG_RUN variable")
if (${DEBUG_RUN})
	add_definitions(-DDEBUG_RUN=1)
endif(${DEBUG_RUN})

# =======================================================================
# Find packages
# =======================================================================

# Add path to cmake dir.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Find Boost package
find_package(Boost 1.54 REQUIRED COMPONENTS system thread random program_options regex date_time serialization)
# Try to include Boost as system directory to suppress it's warnings
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# Find Eigen package
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

find_package( OpenBLAS )
if(NOT OpenBLAS_FOUND)
    message(WARNING "-- OpenBLAS not found!")
else(NOT OpenBLAS_FOUND)
	include_directories(${OpenBLAS_INCLUDE_DIR})
	add_definitions(-DOpenBLAS_FOUND=1)
endif(NOT OpenBLAS_FOUND)

# Find MIC Toolchain
find_package(MIToolchain 1.0 REQUIRED)

# Locate GTest
find_package(GTest)

if(NOT GTEST_FOUND)
    message(WARNING "--   GTest not found!")
else(NOT GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIRS})
	# This must be set in the root directory for the tests to be run by 'make test' or ctest.
	enable_testing()
endif(NOT GTEST_FOUND)

# Add additional option to cmake.
set(BUILD_UNIT_TESTS ON CACHE BOOL "Build unit tests.")

# =======================================================================
# RPATH settings
# =======================================================================
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# =======================================================================
# Cmake configuration
# =======================================================================
# Create the cached variable for storing all library names.
set(MIAlgorithms_LIBRARIES "" CACHE INTERNAL "" FORCE)

# add subdirectory.
add_subdirectory(src)

message ("-- Configured MI Algorithms libraries:\n" "--   " "${MIAlgorithms_LIBRARIES}")

# Set include directory
set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/include ${CMAKE_ADD_INCLUDE_PATH}")
# Set lib directory
set(CMAKE_LIB_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_ADD_LIB_PATH}")
# Set variable that will store generated libraries

# =======================================================================
# Preparation of cmake configs
# =======================================================================

# Configure *Config.cmake and *ConfigVersion.cmake
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/MIAlgorithmsConfig.cmake.in" "${CMAKE_BINARY_DIR}/MIAlgorithmsConfig.cmake" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/MIAlgorithmsConfigVersion.cmake.in" "${CMAKE_BINARY_DIR}/MIAlgorithmsConfigVersion.cmake" @ONLY)

# Install the *Config.cmake and *ConfigVersion.cmake
install(FILES
  "${CMAKE_BINARY_DIR}/MIAlgorithmsConfig.cmake"
  "${CMAKE_BINARY_DIR}/MIAlgorithmsConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/MIAlgorithms/")
  
