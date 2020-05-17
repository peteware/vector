add_executable( unittest
                allocator_traits.t.cpp
                cstddef.t.cpp
                memory.t.cpp
                test_opcounter.cpp
                test_optracker.cpp
                test_permute.cpp
                reverse_iterator.t.cpp
                storage.t.cpp
                uninitialized_move.t.cpp
                unittest.m.cpp
                vector_1.t.cpp
                vector_assign.t.cpp
                vector_assignment.t.cpp
                vector_at.t.cpp
                vector_constructor.t.cpp
                vector_emplace.t.cpp
                vector_erase.t.cpp
                vector_insert.t.cpp
                vector_iter.t.cpp
                vector_push_back.t.cpp
                vector_resize.t.cpp
                )
target_sources ( unittest PUBLIC
                 test_same.h
                 test_opcounter.h
                 test_optracker.h
                 test_permute.h
                 test_random_access_iterator.h
                 test_testtype.h
               )
target_include_directories( unittest PRIVATE ${CMAKE_SOURCE_DIR}/test)
               
target_link_libraries(unittest pw catch2)
add_test( NAME unittest
          COMMAND unittest)

