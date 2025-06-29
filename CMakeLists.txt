cmake_minimum_required(VERSION 3.15)

# set the project name
project(pw VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
# This defines some variables to support best practices for varous paths
include(GNUInstallDirs)
# There is testing
enable_testing()

add_subdirectory(pw)
add_subdirectory(tests)
