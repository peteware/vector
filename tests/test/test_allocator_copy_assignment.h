#ifndef INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H
#define INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H

#include <pw/impl/bool_type.h>

#include "test_base_allocator.h"

namespace pw::test {
template<class Type>
struct allocator_copy_assignment : allocator_base<Type>
{
    using propagate_on_container_copy_assignment = pw::true_type;

    explicit allocator_copy_assignment(int instance = -2);
};

template<class Type>
allocator_copy_assignment<Type>::allocator_copy_assignment(int instance)
    : allocator_base<Type>(instance)
{
}

} // namespace pw::test
#endif /* INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H */
