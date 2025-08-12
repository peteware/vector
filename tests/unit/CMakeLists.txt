add_executable(unittest
        unittest.m.cpp

        allocator_traits.t.cpp
        copy.t.cpp
        cstddef.t.cpp
        distance.t.cpp
        equal.t.cpp
        exchange.t.cpp
        is_constructible.t.cpp
        is_empty.t.cpp
        memory.t.cpp
        move.t.cpp
        reverse_iterator.t.cpp
        storage.t.cpp
        swap.t.cpp
        uninitialized_copy.t.cpp
        uninitialized_move.t.cpp
        vector_1.t.cpp
        vector_access.t.cpp
        vector_assign.t.cpp
        vector_assignment.t.cpp
        vector_capacity.t.cpp
        vector_clear.t.cpp
        vector_compare.t.cpp
        vector_constructor.t.cpp
        vector_constructor_exception_safety.t.cpp
        vector_emplace.t.cpp
        vector_emplace_back.t.cpp
        vector_erase.t.cpp
        vector_insert.t.cpp
        vector_int.t.cpp
        vector_iter.t.cpp
        vector_pop_back.t.cpp
        vector_push_back.t.cpp
        vector_resize.t.cpp
)
#target_include_directories( unittest PRIVATE .. ../test ../.. )

target_link_libraries(unittest pw catch2 pwtest)
add_test(NAME unittest
        COMMAND unittest)
