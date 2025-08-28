#include <test_allocator_only_type.h>

namespace pw::test {

OpCounter AllocatorOnlyType::s_opCounter;

OpCounter
AllocatorOnlyType::getCounter()
{
    return s_opCounter;
}

} // namespace pw::test
