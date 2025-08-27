#ifndef INCLUDED_PW_TEST_NO_ALLOCATOR_TYPE_H
#define INCLUDED_PW_TEST_NO_ALLOCATOR_TYPE_H

#include <test_optracker.h>

namespace pw::test {

struct NoAllocatorType : public OpTracker
{
    static OpCounter getCounter();

    NoAllocatorType();
    explicit NoAllocatorType(value_type const& value);
    NoAllocatorType(value_type const& value, int extra);

private:
    static OpCounter s_opCounter;
};

} // namespace pw::test

#endif /* INCLUDED_PW_TEST_NO_ALLOCATOR_TYPE_H */