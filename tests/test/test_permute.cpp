#include <test_permute.h>

namespace pw::test {

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
        value.push_back(value.back() + static_cast<char>(1));
    }
    return true;
}

bool
permute(float& value, int depth)
{
    if (value >= static_cast<float>(depth))
    {
        return false;
    }
    ++value;
    return true;
}

bool
permute(double& value, int depth)
{
    if (value >= static_cast<double>(depth))
    {
        return false;
    }
    ++value;
    return true;
}

} // namespace pw::test
