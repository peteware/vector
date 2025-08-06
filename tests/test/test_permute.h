#ifndef INCLUDED_PW_TEST_PERMUTE_H
#define INCLUDED_PW_TEST_PERMUTE_H

#include <string>

namespace pw::test {

bool permute(int& value, int depth);
bool permute(std::string& value, int depth);
bool permute(float& value, int depth);
bool permute(double& value, int depth);

template<class Container>
inline bool
permute(Container& value, int depth)
{
    if (value.size() >= depth)
    {
        return false;
    }
    if (value.empty())
    {
        typename Container::value_type val;
        (void)permute(val, depth);
        value.push_back(val);
    }
    else
    {
        typename Container::value_type val(value.back());
        (void)permute(val, depth);
        value.push_back(val);
    }
    return true;
}

template<class Type>
Type
permute_n(Type& value, int depth, int count)
{
    while (count && permute(value, depth))
        --count;

    return value;
}

} // namespace pw::test

#endif /* INCLUDED_PW_TEST_PERMUTE_H */
