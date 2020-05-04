#include "test_permute.h"

namespace pw { namespace test {

bool
permute(int& value, int depth)
{
    if (value >= depth)
    {
        return false;
    }
    ++value;
    return true;
}

bool
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

bool
permute(float& value, int depth)
{
    if (value >= (float)depth)
    {
        return false;
    }
    ++value;
    return true;
}

bool
permute(double& value, int depth)
{
    if (value >= (float)depth)
    {
        return false;
    }
    ++value;
    return true;
}

}} // namespace pw
