add_executable( unittest
                unittest.m.cpp

                allocator_traits.t.cpp
                cstddef.t.cpp
                distance.t.cpp
                memory.t.cpp
                move.t.cpp
                reverse_iterator.t.cpp
                storage.t.cpp
                uninitialized_move.t.cpp
                vector_1.t.cpp
                vector_access.t.cpp
                vector_assign.t.cpp
                vector_assignment.t.cpp
                vector_capacity.t.cpp
                vector_clear.t.cpp
                vector_constructor.t.cpp
                vector_emplace.t.cpp
                vector_erase.t.cpp
                vector_insert.t.cpp
                vector_iter.t.cpp
                vector_pop_back.t.cpp
                vector_push_back.t.cpp
                vector_resize.t.cpp
                )
target_include_directories( unittest PRIVATE .. ../test ../.. )
               
target_link_libraries(unittest pw pwtest catch2)
add_test( NAME unittest
          COMMAND unittest)

