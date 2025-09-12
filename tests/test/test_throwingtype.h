#ifndef PW_TEST_THROWINGTYPE_H
#define PW_TEST_THROWINGTYPE_H

#include <stdexcept>

namespace pw::test {

// Helper class that throws on construction after N instances
class ThrowingType
{
public:
    static int construction_count;
    static int throw_after_n;

    int        value;

    explicit ThrowingType(int value = 0);
    ThrowingType(ThrowingType const& other);
    ThrowingType(ThrowingType&& other) noexcept(false);

    ~ThrowingType();
    static void reset();
};

} // namespace pw::test
#endif
