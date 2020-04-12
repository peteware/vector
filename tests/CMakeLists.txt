add_executable( unittest
                opcounter.cpp
                optracker.cpp
                cstddef.t.cpp
                memory.t.cpp
                permute.cpp
                reverse_iterator.t.cpp
                storage.t.cpp
                uninitialized_move.t.cpp
                unittest.m.cpp
                vector.t.cpp
                vector_insert.t.cpp
                vector_assign.t.cpp
                vector_assignment.t.cpp
                vector_1.t.cpp
                vector_constructor.t.cpp
                )
target_link_libraries(unittest pw)
add_test( NAME unittest
          COMMAND unittest)

