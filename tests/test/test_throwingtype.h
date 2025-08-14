#ifndef PW_TEST_THROWINGTYPE_H
#define PW_TEST_THROWINGTYPE_H

#include <stdexcept>

namespace pw { namespace test {

// Helper class that throws on construction after N instances
class ThrowingType
{
public:
    static int construction_count;
    static int throw_after_n;

    int        value;

    explicit ThrowingType(int v = 0);
    ThrowingType(const ThrowingType& other);
    ThrowingType(ThrowingType&& other) noexcept(false);

    ~ThrowingType();
    static void reset();
};

}} // namespace pw::test
#endif
