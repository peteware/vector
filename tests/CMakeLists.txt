add_executable( unittest
                conscounter.cpp
                copyconstructible.cpp
                pw_cstddef.cpp
                pw_memory.t.cpp
                pw_vector.cpp
                pw_vector.m.t.cpp
                pw_vector.t.cpp
                pw_vector_constructor.t.cpp
                pw_internal_allocate.t.cpp
                pw_internal_storage.t.cpp
                )
target_link_libraries(unittest pw)
add_test( NAME unittest
          COMMAND unittest)

