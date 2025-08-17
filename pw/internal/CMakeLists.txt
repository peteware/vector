add_library(pw-internal INTERFACE
        unimplemented.h
        rsize_fix.h)

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
        meta.h
        storage.h
)
target_link_libraries(pw-internal INTERFACE pw-impl)
