Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(unittest
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
        vector_iter.t.cpp
        vector_pop_back.t.cpp
        vector_push_back.t.cpp
        vector_reserve.t.cpp
        vector_resize.t.cpp
        vector_shrink_to_fit.t.cpp
        vector_swap.t.cpp
)
#target_include_directories( unittest PRIVATE .. ../test ../.. )

target_link_libraries(unittest pw pwtest Catch2::Catch2WithMain)
add_test(NAME unittest
        COMMAND unittest)
