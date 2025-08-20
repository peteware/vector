#ifndef PW_TEST_ALLOCATOR_IS_ALWAYS_EQUAL_H
#define PW_TEST_ALLOCATOR_IS_ALWAYS_EQUAL_H

#include <test_allocator_base.h>

namespace pw::test {
template<class Type>
struct allocator_is_always_equal : public allocator_base<Type>
{
    using is_always_equal = true_type;

    explicit allocator_is_always_equal(int instance = -3);
};

template<class Type>
allocator_is_always_equal<Type>::allocator_is_always_equal(int instance)
    : allocator_base<Type>(instance)
{
}

} // namespace pw::test
#endif //PW_TEST_ALLOCATOR_IS_ALWAYS_EQUAL_H
