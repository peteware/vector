add_library(pw-internal
        memory_resource_new_delete.cpp
        memory_resource_null.cpp
)

target_include_directories(
        pw-internal
        INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/pw/internal>)

target_sources(pw-internal INTERFACE
        compare.h
        constructible.h
        detect_prop.h
        extract_or.h
        is_supported.h
        memory_resource_new_delete.h
        memory_resource_null.h
        meta.h
        rsize_fix.h
        storage.h
        unimplemented.h
)
target_link_libraries(pw-internal INTERFACE pw-impl)
