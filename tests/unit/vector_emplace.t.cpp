//#include <pw/impl/forward.h>
#include <test_emplacemoveconstructible.h>
#include <test_testtype.h>

#include <catch2/catch.hpp>

namespace pw {
template<class Type, class Allocator>
struct Rai
{
    Rai(internal::Storage<Type, Allocator>* orig, internal::Storage<Type, Allocator>* n = 0)
        : m_orig(orig)
        , m_new(n)
    {
    }

    ~Rai()
    {
        if (m_new && m_orig)
        {
            swap(*m_orig, *m_new);
        }
    }

    internal::Storage<Type, Allocator>* m_orig;
    internal::Storage<Type, Allocator>* m_new;
};

template<class Type, class Allocator>
template<class... Args>
typename vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    if (!m_data.hascapacity())
    {
        Storage s(pw::move(m_data), m_data.newsize(), m_data.get_allocator());
        m_data.swap(s);
    }
    allocator_traits<Allocator>::construct(m_data.get_allocator(), m_data.end(), std::forward<Args>(args)...);
    m_data.set_size(m_data.size() + 1);
    return *(m_data.end() - 1);
}

template<class Type, class Allocator>
template<class... Args>
typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    if (!m_data.hascapacity())
    {
        m_data = Storage(pw::move(m_data), m_data.newsize());
    }
    return begin();
}

} // namespace pw

using TestIntList = std::tuple<pw::vector<int>, std::vector<int>>;
// using TestEmplaceList = std::tuple<pw::vector<pw::test::EmplaceMoveConstructible>,
//                                    std::vector<pw::test::EmplaceMoveConstructible>>;
using TestEmplaceList = std::tuple<pw::vector<pw::test::EmplaceMoveConstructible>>;

/*
 * Type requirements:
 * - emplace_back(): MoveInsertable and EmplaceConstructible
 * - emplace(): MoveAssignable, MoveInsertable and EmplaceConstructible
 */
TEMPLATE_LIST_TEST_CASE("emplace_back() with EmplaceMoveConstructible",
                        "[vector][emplace_back]",
                        TestEmplaceList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector              v;
        pw::test::OpCounter init = pw::test::EmplaceMoveConstructible::getCounter();
        pw::test::OpCounter counter;

        WHEN("emplace-back() an element")
        {
            v.emplace_back(3, 4);
            counter = pw::test::EmplaceMoveConstructible::getCounter() - init;
            THEN("size() is 1")
            {
                REQUIRE(1 == v.size());
            }
            THEN("element has correct values")
            {
                REQUIRE(3 == v.front().value());
                REQUIRE(4 == v.front().value2());
            }
            THEN("other constructed, not copy or moce")
            {
                REQUIRE(1 == counter.getOtherConstructor());
                REQUIRE(1 == counter.constructorCount());
            }
        }
    }
    GIVEN("A vector with elements")
    {
        Vector              v    = { { 1, 2 }, { 3, 4 }, { 4, 5 } };
        pw::test::OpCounter init = pw::test::EmplaceMoveConstructible::getCounter();
        pw::test::OpCounter counter;

        REQUIRE(3 == v.size());
        WHEN("emplace-back() an element")
        {
            v.emplace_back(13, 14);
            counter = pw::test::EmplaceMoveConstructible::getCounter() - init;
            THEN("size() is 4")
            {
                REQUIRE(4 == v.size());
            }
            THEN("element has correct values")
            {
                REQUIRE(1 == v.front().value());
                REQUIRE(2 == v.front().value2());
                REQUIRE(13 == v.back().value());
                REQUIRE(14 == v.back().value2());
            }
            THEN("other constructed, not copy or move")
            {
                INFO("counter = " << counter);
                REQUIRE(1 == counter.getOtherConstructor());
                REQUIRE(3 == counter.getMoveConstructor());
                REQUIRE(4 == counter.constructorCount());
            }
        }
    }
}

#if 0

TEMPLATE_LIST_TEST_CASE("emplace_back()", "[vector][emplace_back]", TestIntList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector of TestType")
    {
        Vector v;
        WHEN("emplace_back() and item")
        {
            v.emplace_back(3);
            THEN("size() is 1")
            {
                REQUIRE(1 == v.size());
            }
        }
    }
    GIVEN("A vector with 5 elements")
    {
        pw::test::Values<Vector> generate(5);
        Vector&        v = generate.values;

        WHEN("emplace_back()")
        {
        }
    }
}
#endif
