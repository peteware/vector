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
            THEN("it is empty") { REQUIRE(empty); }
        }
    }
    GIVEN("A vector of count values")
    {
        pw::size_t const total = 10;
        value_type       value = 3;
        Vector           v(total, value);

        WHEN("It is initialized with value")
        {
            bool empty = v.empty();
            THEN("not empty") { REQUIRE(!empty); }
            THEN("size is total") { REQUIRE(total == v.size()); }
        }
    }
}
