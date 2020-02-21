#ifndef INCLUDED_PW_IMPL_EQUAL_H
#define INCLUDED_PW_IMPL_EQUAL_H

namespace pw {

template<class Iterator1, class Iterator2>
constexpr bool
equal(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2)
{
    while (begin1 != end1 && begin2 != end2)
    {
        if (!(*begin1 == *begin2))
        {
            return false;
        }
        ++begin1;
        ++begin2;
    }
    return begin1 == end1 && begin2 == end2;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_EQUAL_H */
