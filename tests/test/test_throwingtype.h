#ifndef PW_TEST_THROWINGTYPE_H
#define PW_TEST_THROWINGTYPE_H

#include <stdexcept>

namespace pw {
namespace test {

// Helper class that throws on construction after N instances
class ThrowingType
{
public:
    static int  construction_count;
    static int  throw_after_n;
    static bool should_throw_on_copy;
    static bool should_throw_on_move;

    int         value;

    explicit ThrowingType(int v = 0)
        : value(v)
    {
        if (++construction_count > throw_after_n && throw_after_n > 0)
        {
            --construction_count;
            throw std::runtime_error("Construction limit reached");
        }
    }

    ThrowingType(const ThrowingType& other)
        : value(other.value)
    {
        if (should_throw_on_copy || (++construction_count > throw_after_n && throw_after_n >= 0))
        {
            --construction_count;
            throw std::runtime_error("Copy construction failed");
        }
    }

    ThrowingType(ThrowingType&& other) noexcept(false)
        : value(other.value)
    {
        if (should_throw_on_move || (++construction_count > throw_after_n && throw_after_n >= 0))
        {
            --construction_count;
            throw std::runtime_error("Move construction failed");
        }
    }

    ~ThrowingType() { --construction_count; }

    static void reset()
    {
        construction_count   = 0;
        throw_after_n        = -1;
        should_throw_on_copy = false;
        should_throw_on_move = false;
    }
};

// Static member definitions
inline int  ThrowingType::construction_count   = 0;
inline int  ThrowingType::throw_after_n        = -1;
inline bool ThrowingType::should_throw_on_copy = false;
inline bool ThrowingType::should_throw_on_move = false;

} // namespace test
} // namespace pw

#endif // PW_TEST_THROWINGTYPE_H