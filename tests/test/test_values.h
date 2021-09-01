#ifndef _INCLUDED_TEST_VALUES_H
#define _INCLUDED_TEST_VALUES_H

namespace pw { namespace test {

/**
 * This provides a simple way to populate a container
 * with a fixed number of values.  It saves
 * how mnany values in `count` and the
 * `first_value` and `last_value`. 
 *
 * It uses permute() to generate unique values.
 */
template<class Container>
struct Values
{
    using value_type = typename Container::value_type;
    value_type   first_value;
    value_type   last_value;
    size_t const count;
    Container    values;

    template<class... Args>
    Values(size_t total, Args&&... args)
        : count(total)
        , values(pw::forward<Args>(args)...)
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

template<class Container>
std::ostream&
operator<<(std::ostream& out, Values<Container> const& values)
{
    out << "#### size = " << values.values.size() << " ###\n";
    auto iter = values.values.begin();
    for (size_t i = 0; i < values.count; ++i)
    {
        out << i << " = " << *iter++ << '\n';
    }
    return out;
}

}} // namespace pw::test
#endif /* _INCLUDED_TEST_VALUES_H */
