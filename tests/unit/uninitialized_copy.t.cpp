#include <pw/impl/destroy.h>
#include <pw/impl/uninitialized_copy.h>
#include <tests/catch2/catch.hpp>
#include <tests/test/test_throwingtype.h>

SCENARIO("uninitialized_copy() handles exceptions", "[uninitialized_copy]")
{
    using pw::test::ThrowingType;
    GIVEN("An initialized source array and uninitialized destination buffer")
    {
        ThrowingType::reset();
        ThrowingType::throw_after_n = -1; // No exceptions by default
        ThrowingType src[5]         = {
            ThrowingType(1), ThrowingType(2), ThrowingType(3), ThrowingType(4), ThrowingType(5)
        };
        alignas(ThrowingType) unsigned char dest_buf[sizeof(ThrowingType) * 5] = {};
        ThrowingType*                       dest = reinterpret_cast<ThrowingType*>(dest_buf);
        ThrowingType::reset();
        WHEN("ThrowingType raises exception after 2 constructions")
        {
            ThrowingType::throw_after_n = 2; // Throw after 2 objects are constructed
            REQUIRE_THROWS(pw::uninitialized_copy(&src[0], &src[5], dest));
            // Only throw_after_n objects should have been constructed, and then destroyed
            THEN("every object is destructed")
            {
                REQUIRE(ThrowingType::construction_count == 0);
            }
            THEN("value is set to destroyed values")
            {
                REQUIRE(dest[0].value == -2);
                REQUIRE(dest[1].value == -2);
            }
        }
    }
}
