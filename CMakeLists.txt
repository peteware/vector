cmake_minimum_required(VERSION 3.15)

# set the project name
project(pw VERSION 1.0 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
# This defines some variables to support best practices for varous paths
include(GNUInstallDirs)

# Add the library as an interface
add_library(${PROJECT_NAME} INTERFACE)

# Now say how to get the headers
target_include_directories( ${PROJECT_NAME} INTERFACE
                            $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#add_subdirectory(pw)
add_subdirectory(tests)
add_subdirectory(tests/catch2)
