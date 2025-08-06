#ifndef INCLUDED_PW_TEST_ALLOCATOR_MOVE_ASSIGNMENT_H
#define INCLUDED_PW_TEST_ALLOCATOR_MOVE_ASSIGNMENT_H

#include <pw/impl/bool_type.h>

#include "test_base_allocator.h"

namespace pw::test {

template<class Type>
struct allocator_move_assignment : public allocator_base<Type>
{
    using propagate_on_container_move_assignment = pw::true_type;

    allocator_move_assignment(int instance = -1);
};

template<class Type>
allocator_move_assignment<Type>::allocator_move_assignment(int instance)
    : allocator_base<Type>(instance)
{
}

} // namespace pw::test
#endif /* INCLUDED_PW_TEST_ALLOCATOR_MOVE_ASSIGNMENT_H */
