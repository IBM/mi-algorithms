# Project name
project(MIAlgorithms CXX)

#  Set current version number.
set (MIAlgorithms_VERSION_MAJOR 1)
set (MIAlgorithms_VERSION_MINOR 0)
set (MIAlgorithms_VERSION_PATCH 0)
set (MIAlgorithms_VERSION ${MIAlgorithms_VERSION_MAJOR}.${MIAlgorithms_VERSION_MINOR}.${MIAlgorithms_VERSION_PATCH})

# CMake required version.
cmake_minimum_required(VERSION 3.2)

# Add C++11 dependency.
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall")


# =======================================================================
# Create CMAKE variables storing preprocessor commands
# =======================================================================

# Default length of SDR.
set(DEFAULT_SDR_LENGTH 1024 CACHE STRING "Default length of SDR")
add_definitions(-DDEFAULT_SDR_LENGTH=${DEFAULT_SDR_LENGTH})

# Debug (DEPRICATED - TO BE REMOVED!
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
find_package(Boost 1.58 REQUIRED COMPONENTS system thread random program_options regex date_time)
# Try to include Boost as system directory to suppress it's warnings
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# Find OpenBLAS package
#find_package( OpenBLAS REQUIRED )
#include_directories(${OpenBLAS_INCLUDE_DIR})

# Find Eigen package
find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

# Find MIC Toolchain
find_package(MIToolchain REQUIRED)

message("Found MI Toolchain libs: ${MIToolchain_LIBRARIES} ")

# =======================================================================
# RPATH settings.
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


add_subdirectory(src)

# =======================================================================
# MIAlgorithmsConfig.cmake preparation
# =======================================================================

# include directories
set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE 
	"${CMAKE_INSTALL_PREFIX}/include"
)

# lib directory

set(CMAKE_LIB_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/lib")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/MIAlgorithmsConfig.cmake.in" "${CMAKE_BINARY_DIR}/MIAlgorithmsConfig.cmake" IMMEDIATE @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/MIAlgorithmsConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/share/MIAlgorithms/)

# =======================================================================
# CPack: binary installer generator
# =======================================================================

# CPack variables used when generating binary installer
set(CPACK_PACKAGE_NAME "MIAlgorithms")
set(CPACK_PACKAGE_VENDOR "MIAlgorithms")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MIAlgorithms - Machine Intelligence Toolchain")
set(CPACK_PACKAGE_VERSION ${MIAlgorithms_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${MIAlgorithms_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MIAlgorithms_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MIAlgorithms_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MIAlgorithms")

# License file
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# Tell CPack all of the components to install. The "ALL"
# refers to the fact that this is the set of components that
# will be included when CPack is instructed to put everything
# into the binary installer (the default behavior).
set(CPACK_COMPONENTS_ALL system_utils logger configuration application)

# This must always be last!
include(CPack)
