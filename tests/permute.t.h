#ifndef INCLUDED_PW_PERMUTE_T_H
#define INCLUDED_PW_PERMUTE_T_H

#include <string>

namespace pw { namespace internal {

inline bool
permute(int& value, int depth)
{
    if (value >= depth)
    {
        return false;
    }
    ++value;
    return true;
}

inline bool
permute(std::string& value, int depth)
{
    if (value.size() >= depth)
    {
        return false;
    }
    if (value.empty())
    {
        value.push_back('a');
    }
    else
    {
        value.push_back(value.back() + 1);
    }
    return true;
}

inline bool
permute(float& value, int depth)
{
    if (value >= (float)depth)
    {
        return false;
    }
    ++value;
    return true;
}
inline bool
permute(double& value, int depth)
{
    if (value >= (float)depth)
    {
        return false;
    }
    ++value;
    return true;
}

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
}} // namespace pw::internal

#endif /* INCLUDED_PW_PERMUTE_T_H */
