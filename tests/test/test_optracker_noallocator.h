#ifndef INCLUDED_PW_TEST_OPTRACKER_NOALLOCATOR_H
#define INCLUDED_PW_TEST_OPTRACKER_NOALLOCATOR_H

#include <test_optracker.h>

namespace pw::test {

struct OpTrackerNoAllocator : public OpTracker
{
    static OpCounter getCounter();

    OpTrackerNoAllocator();
    explicit OpTrackerNoAllocator(value_type const& value);
    OpTrackerNoAllocator(value_type const& value, int extra);

private:
    static OpCounter s_opCounter;
};

} // namespace pw::test

#endif /* INCLUDED_PW_TEST_OPTRACKER_NOALLOCATOR_H */