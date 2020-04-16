add_executable( unittest
                cstddef.t.cpp
                memory.t.cpp
                opcounter.cpp
                optracker.cpp
                permute.cpp
                reverse_iterator.t.cpp
                storage.t.cpp
                uninitialized_move.t.cpp
                unittest.m.cpp
                vector_erase.t.cpp
                vector.t.cpp
                vector_1.t.cpp
                vector_assign.t.cpp
                vector_assignment.t.cpp
                vector_at.t.cpp
                vector_constructor.t.cpp
                vector_emplace.t.cpp
                vector_insert.t.cpp
                vector_push_back.t.cpp
                vector_resize.t.cpp
                )
target_link_libraries(unittest pw)
add_test( NAME unittest
          COMMAND unittest)

