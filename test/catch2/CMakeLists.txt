add_library(catch2 INTERFACE)

target_sources(catch2 INTERFACE
               catch.hpp)

target_include_directories(catch2 INTERFACE
                            $<BUILD_INTERFACE:${catch2_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
