#include <pw/internal/storage.h>

#include <catch2/catch.hpp>
#include <test_defaultcopyconstructible.h>
#include <test_opcounter.h>
#include <test_permute.h>
#include <test_values.h>

//using TestTypeList = std::tuple<int, double>;
using TestTypeList = std::tuple<int>;

TEMPLATE_LIST_TEST_CASE("check impl/storage", "[storage]", TestTypeList)
{
    using Allocator = pw::allocator<TestType>;
    using Storage   = pw::internal::Storage<TestType, Allocator>;
    Allocator allocator;

    GIVEN("An empty Storage")
    {
        Storage storage(allocator);
        WHEN("nothing is changed")
        {
            THEN("it's all empty")
            {
                REQUIRE((pw::size_t)0 == storage.size());
                REQUIRE((pw::size_t)0 == storage.capacity());
                REQUIRE(storage.begin() == storage.end());
            }
            THEN("newsize() is larger")
            {
                REQUIRE(storage.newsize() > 0);
            }
        }
    }
    GIVEN("An allocated Storage with 10")
    {
        Storage storage(10, allocator);
        // WHEN("Storage(move, 20)")
        // {
        //     Storage s = Storage(pw::move(storage), 20, typename Storage::allocator_type());
        //     THEN("size() is 0 but capacity increased")
        //     {
        //         REQUIRE((pw::size_t)0 == s.size());
        //         REQUIRE((pw::size_t)20 == s.capacity());
        //     }
        //     THEN("storage is empty")
        //     {
        //         REQUIRE(0 == storage.size());
        //         REQUIRE((pw::size_t)10 == storage.capacity());
        //     }
        // }
        WHEN("newsize() is called")
        {
            THEN("newsize() is larger")
            {
                REQUIRE(storage.newsize() > 10);
            }
        }
    }
    GIVEN("An allocated Storage with 10")
    {
        Storage  storage(10, allocator);
        TestType value;
        pw::test::permute(value, 3);
        WHEN("push_back(value)")
        {
            storage.push_back(value);
            THEN("size() == 1")
            {
                REQUIRE(1 == storage.size());
            }
            THEN("capacity() == 10")
            {
                REQUIRE(10 == storage.capacity());
            }
            THEN("*begin() == value")
            {
                REQUIRE(value == *storage.begin());
            }
        }
    }
    GIVEN("An allocated Storage with 10 and one element")
    {
        Storage  storage(10, allocator);
        TestType value;
        pw::test::permute(value, 3);
        storage.push_back(value);
        REQUIRE(*storage.begin() == value);
        WHEN("swap() is called")
        {
            size_t  capacity = storage.capacity();
            Storage s(0, allocator);
            swap(storage, s);
            THEN("s now has 1 element")
            {
                REQUIRE(s.size() == 1);
            }
            THEN("the element was copied")
            {
                REQUIRE(*s.begin() == value);
            }
            THEN("capacity was copied")
            {
                REQUIRE(s.capacity() == capacity);
            }
            THEN("storage now has 0 element")
            {
                REQUIRE(storage.size() == 0);
            }
        }
    }
}

SCENARIO("Storage construct counts", "[storage][count]")
{
    using Storage                  = pw::internal::Storage<pw::test::DefaultCopyConstructible>;

    pw::test::OpCounter const init = pw::test::DefaultCopyConstructible::getCounter();
    pw::test::OpCounter       counter;
    GIVEN("An empty Storage")
    {
        Storage storage;
        WHEN("Nothing happens")
        {
            REQUIRE(storage.empty());
            counter = pw::test::DefaultCopyConstructible::getCounter() - init;
            REQUIRE(counter.zero());
        }
        WHEN("Allocate items")
        {
            Storage s(10);
            REQUIRE(s.capacity() == 10);
        }
    }
    GIVEN("A Storage with one item")
    {
        Storage storage(5);
        WHEN("item is push_back()")
        {
            storage.push_back(1);
            THEN("it is added")
            {
                REQUIRE(1 == storage.size());
                REQUIRE(1 == storage.begin()->value());
            }
        }
    }

    counter = pw::test::DefaultCopyConstructible::getCounter() - init;
    REQUIRE(counter.constructorCount() == counter.destructorCount());
}

SCENARIO("Storage move construct counts", "[storage][move]")
{
    using Storage                     = pw::internal::Storage<pw::test::DefaultCopyConstructible>;

    int const                 size    = 5;
    int const                 genSize = size - 2;
    Storage                   storage(size);
    pw::test::Values<Storage> gen(genSize, genSize);
    pw::test::OpCounter       counter;

    GIVEN("A Storage with size element")
    {
        pw::test::OpCounter init = pw::test::DefaultCopyConstructible::getCounter();
        WHEN("moveto()")
        {
            storage.moveto(gen.values.begin(), gen.values.begin() + 2, storage.begin() + 2);
            counter = pw::test::DefaultCopyConstructible::getCounter() - init;
            THEN("Only move constructor is called")
            {
                INFO("counter: " << counter << " storage[1] = " << *(storage.begin() + 1));
                REQUIRE(2 == counter.getMoveConstructor());
                REQUIRE(2 == counter.constructorCount());
                REQUIRE(gen.first_value == *storage.begin());
                // OpTracker move constructor does: value *= -2
                REQUIRE(*(gen.values.begin() + 1) == *(storage.begin() + 1));
            }
        }
    }
}
