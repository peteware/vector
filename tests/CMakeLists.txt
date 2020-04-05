add_executable( unittest
                conscounter.cpp
                copyconstructible.cpp
                cstddef.t.cpp
                memory.t.cpp
                reverse_iterator.t.cpp
                storage.t.cpp
                uninitialized_move.t.cpp
                unittest.m.cpp
                vector.t.cpp
                vector_1.t.cpp
                vector_constructor.t.cpp
                )
target_link_libraries(unittest pw)
add_test( NAME unittest
          COMMAND unittest)

