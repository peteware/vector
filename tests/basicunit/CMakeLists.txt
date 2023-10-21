add_executable( basictest
                basictest.m.cpp

                basicunit_allocator.t.cpp
                vector_int.t.cpp
                )
#target_include_directories( unittest PRIVATE .. ../test ../.. )
               
target_link_libraries(basictest pw catch2 pwtest)
add_test( NAME basictest
          COMMAND basictest)

