#ifndef INCLUDED_PW_TEST_TESTTYPE_H
#define INCLUDED_PW_TEST_TESTTYPE_H
#include <pw/vector>
#include <string>
#include <tuple>
#include <vector>

#include <test_permute.h>

namespace pw { namespace test {
using TestTypeList = std::tuple<pw::vector<int>, pw::vector<std::string>, std::vector<int>>;

template<class Container>
struct Values
{
    using value_type = typename Container::value_type;
    value_type   first_value;
    value_type   last_value;
    size_t const count;
    Container    values;

    Values(size_t total)
        : count(total)
    {
        value_type value;

        for (size_t i = 0; i < count; ++i)
        {
            pw::test::permute(value, count);
            if (i == 0)
                first_value = value;
            values.push_back(value);
            last_value = value;
        }
    }
};
}} // namespace pw

#endif /*  INCLUDED_PW_TEST_TESTTYPE_H */
