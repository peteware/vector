#ifndef INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H
#define INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H

#include <pw/impl/type_traits/bool_type.h>
#include <test_allocator_base.h>

namespace pw::test {

/**
 * Same as `allocator_base` but sets `propogate_on_container_copy_assignment` to true
 *
 * You should initialize it with a value other than -2 (the default)
 * and then check that m_instance is different
 *
 * @tparam Type What is being allocated
 */
template<class Type>
struct allocator_copy_assignment : allocator_base<Type>
{
    using propagate_on_container_copy_assignment = true_type;

    explicit allocator_copy_assignment(int instance = -2);
};

template<class Type>
allocator_copy_assignment<Type>::allocator_copy_assignment(int instance)
    : allocator_base<Type>(instance)
{
}

} // namespace pw::test
#endif /* INCLUDED_PW_TEST_ALLOCATOR_COPY_ASSIGNMENT_H */
