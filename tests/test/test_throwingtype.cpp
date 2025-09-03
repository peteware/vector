#include <test_throwingtype.h>

namespace pw::test {

int ThrowingType::construction_count;
int ThrowingType::throw_after_n;

ThrowingType::ThrowingType(int value)
    : value(value)
{
    if (++construction_count > throw_after_n && throw_after_n > 0)
    {
        --construction_count;
        throw std::runtime_error("Construction limit reached");
    }
}

ThrowingType::ThrowingType(ThrowingType const& other)
    : value(other.value)
{
    if (++construction_count > throw_after_n && throw_after_n > 0)
    {
        --construction_count;
        throw std::runtime_error("Copy construction failed");
    }
}

ThrowingType::ThrowingType(ThrowingType&& other) noexcept(false)
    : value(other.value)
{
    other.value = -other.value;
    if (++construction_count > throw_after_n && throw_after_n > 0)
    {
        --construction_count;
        throw std::runtime_error("Move construction failed");
    }
}

ThrowingType::~ThrowingType()
{
    --construction_count;
    value = -2;
}

void
ThrowingType::reset()
{
    construction_count = 0;
    throw_after_n      = -1;
}
}; // namespace pw::test
