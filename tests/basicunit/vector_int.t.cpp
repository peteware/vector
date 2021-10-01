#include <pw/vector>

#include <catch2/catch.hpp>

/*
 * Type Requirements:
 * - No extra
 */
TEST_CASE("Constructors", "[vector][constructor]")
{
    using Vector     = pw::vector<int>;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector v;
        WHEN("Check if empty")
        {
            bool empty = v.empty();
            ;
            THEN("it is empty") { REQUIRE(empty); }
        }
    }
}
