#ifndef INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_NONE_H
#define INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_NONE_H

#include <test_optracker.h>

namespace pw::test {

struct OpTrackerAllocatorNone : OpTracker
{
    static OpCounter& getCounter();

    OpTrackerAllocatorNone();
    OpTrackerAllocatorNone(OpTrackerAllocatorNone const& copy);
    explicit OpTrackerAllocatorNone(value_type const& value);
    OpTrackerAllocatorNone(value_type const& value, int extra);

private:
    static OpCounter s_opCounter;
};

} // namespace pw::test

#endif /* INCLUDED_PW_TEST_OPTRACKER_ALLOCATOR_NONE_H */