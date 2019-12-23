cmake_minimum_required(VERSION 3.10)

# set the project name
project(pw VERSION 1.0 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_library(pw INTERFACE)
add_subdirectory(pw)
add_subdirectory(tests)

enable_testing ()
add_test (NAME MyTest COMMAND Test)
