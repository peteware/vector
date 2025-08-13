#include <pw/impl/reverse_iterator.h>
#include <pw/impl/size.h>

#include <catch2/catch_test_macros.hpp>

SCENARIO("check reverse_iterator")
{
    constexpr pw::size_t count        = 4;
    int                  array[count] = { 3, 1, 5, 6 };
    GIVEN("An array count ints")
    {
        WHEN("get an end iterator")
        {
            const pw::reverse_iterator iter { &array[count] };
            THEN("iter is end")
            {
                REQUIRE(array[count - 1] == *iter);
            }
        }
    }
    GIVEN("reverse iterator at end")
    {
        pw::reverse_iterator iter(&array[count]);
        WHEN("increment iterator")
        {
            ++iter;
            THEN("iter is prev")
            {
                REQUIRE(array[count - 2] == *iter);
            }
        }
        WHEN("post increment iterator")
        {
            ++iter;
            THEN("iter is prev")
            {
                REQUIRE(array[count - 2] == *iter);
            }
        }
        WHEN("operator+=()")
        {
            iter += 3;
            THEN("iter is 3")
            {
                REQUIRE(array[count - 4] == *iter);
            }
        }
    }
}
