#include <test_permute.h>

namespace pw::test {

/**
 * Increment value until it is >= depth
 *
 * @param value A reference
 * @param depth Max that value should reach
 * @return true while value can still be incremeneted
 */
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

/**
 * Add a character value until it's size is >= depth
 *
 * @param value A reference
 * @param depth Max size value should reach
 * @return true if permute()'d a different value, false otherwise
 */
bool
permute(std::string& value, int depth)
{
    if (value.size() >= depth)
    {
        return false;
    }
    char const ch = value.empty() ? 'a' : static_cast<char>('a' + (value.back() - 'a' + 1) % 26);
    value.push_back(ch);
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
