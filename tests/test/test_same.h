#ifndef INCLUDED_PW_TEST_SAME_H
#define INCLUDED_PW_TEST_SAME_H

namespace pw::test {

/**
 * @brief Checks if all elements in a range are equal to a given value.
 *
 * Iterates through the range [begin, end) and compares each element with
 * the provided value using operator==. Returns true if all elements are
 * equal to the value, or true for empty ranges.
 *
 * @tparam Iterator An input iterator type that can be dereferenced and incremented.
 *                  The dereferenced type must be comparable with Type using operator==.
 * @tparam Type The type of the value to compare against. Must be comparable with
 *              the iterator's value type using operator==.
 *
 * @param begin Iterator to the beginning of the range to check.
 * @param end Iterator to one past the end of the range to check.
 * @param value The value to compare all elements against.
 *
 * @return true if all elements in [begin, end) are equal to value, or if the
 *         range is empty (begin == end). false if any element differs from value.
 *
 * @note For empty ranges (begin == end), this function returns true.
 * @note The function uses operator== for comparison, so both the iterator's
 *       value type and Type must support this operator.
 *
 * @par Example:
 * @code
 * std::vector<int> v = {5, 5, 5, 5};
 * bool result = same(v.begin(), v.end(), 5);  // returns true
 *
 * std::vector<int> v2 = {5, 5, 3, 5};
 * bool result2 = same(v2.begin(), v2.end(), 5);  // returns false
 * @endcode
 */
template<class Iterator, class Type>
bool
same(Iterator begin, Iterator end, Type const& value)
{
    while (begin != end)
    {
        if (!(*begin++ == value))
        {
            return false;
        }
    }
    return true;
}
} // namespace pw::test
#endif /* INCLUDED_PW_TEST_SAME_H */
