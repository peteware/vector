add_library(${PROJECT_NAME}
        pw.cpp

        algorithm
        allocator
        cstddef
        memory
        memory_resource
        type_traits
        utility
        vector
)

add_subdirectory(internal)
add_subdirectory(impl)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/pw>)

target_link_libraries(${PROJECT_NAME} PRIVATE pw-impl pw-internal)
