add_executable( unittest
                pw_cstddef.cpp
                pw_memory.t.cpp
                pw_vector.cpp
                pw_vector.m.t.cpp
                pw_vector.t.cpp
                )
target_link_libraries(unittest pw)
add_test( NAME unittest
          COMMAND unittest)

