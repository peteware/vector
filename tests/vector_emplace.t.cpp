#include "catch2/catch.hpp"
#include "optracker.h"
#include "permute.h"
#include "same.t.h"
#include "testtype.h"
#include <pw/algorithm>
//#include <pw/allocator>
#include <pw/type_traits>
#include <pw/vector>

#include <pw/impl/forward.h>

namespace pw {

template<class Type, class Allocator>
template<class... Args>
typename vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    if (!m_data.hascapacity())
    {
        m_data = Storage(pw::move(m_data), m_data.newsize());
    }
    allocator_traits<Allocator>::construct(m_data.get_allocator(), m_data.end(), std::forward<Args>(args)...);
    //allocator_traits<Allocator>::construct(m_data.get_allocator(), m_data.end(), 4, 5);
}

template<class Type, class Allocator>
template<class... Args>
typename vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    return begin();
}

} // namespace pw

using TestIntList = std::tuple<pw::vector<int>, std::vector<int>>;
using TestEmplaceList =
    std::tuple<pw::vector<EmplaceMoveConstructible>, std::vector<EmplaceMoveConstructible>>;

TEMPLATE_LIST_TEST_CASE("emplace_back() with EmplaceMoveConstructible",
                        "[vector][emplace_back]",
                        TestEmplaceList)
{
    using Vector     = TestType;
    using value_type = typename Vector::value_type;

    GIVEN("An empty vector")
    {
        Vector v;

        WHEN("emplace-back() an element")
        {
            //v.emplace_back(3, 4);
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
        Values<Vector> generate(5);
        Vector&        v = generate.values;

        WHEN("emplace_back()")
        {
        }
    }
}
#endif
