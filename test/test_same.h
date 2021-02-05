#ifndef INCLUDED_TEST_SAME_H
#define INCLUDED_TEST_SAME_H

namespace pw { namespace test {
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
}} // namespace pw::test
#endif /* INCLUDED_TEST_SAME_H */
