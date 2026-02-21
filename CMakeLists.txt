cmake_minimum_required(VERSION 3.31)

# set the project name
project(pw VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Helps with gcc-15 on MacOS
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
# This defines some variables to support best practices for varous paths
include(GNUInstallDirs)
# There is testing
enable_testing()

add_subdirectory(pw)
add_subdirectory(tests)
