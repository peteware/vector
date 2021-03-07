add_library(catch2 INTERFACE)

target_sources(catch2 INTERFACE
               catch.hpp)

target_include_directories(catch2 INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tests>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/catch2>)
