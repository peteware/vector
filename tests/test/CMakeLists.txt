add_library( pwtest
                test_copyconstructible.cpp
                test_defaultconstructible.cpp
                test_defaultcopyconstructible.cpp
                test_emplacemoveconstructible.cpp
                test_moveconstructible.cpp
                test_opcounter.cpp
                test_optracker.cpp
                test_permute.cpp
                )
target_sources ( pwtest PUBLIC
                test_copyconstructible.h
                test_defaultconstructible.h
                test_defaultcopyconstructible.h
                test_emplacemoveconstructible.h
                test_moveconstructible.h
                test_opcounter.h
                test_optracker.h
                test_permute.h
                test_random_access_iterator.h
                test_same.h
                test_testtype.h
               )
target_link_libraries( pwtest pw )

target_include_directories( pwtest
                            PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
