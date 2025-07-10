#ifndef INCLUDED_PW_INTERNAL_COMPARE_H
#define INCLUDED_PW_INTERNAL_COMPARE_H

namespace pw::internal {
template<class Iter1, class Iter2>
int
compare(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
{
    while (begin1 != end1 && begin2 != end2)
    {
        if (*begin1 < *begin2)
        {
            return -1;
        }
        if (*begin2 < *begin1)
        {
            return 1;
        }
        ++begin1;
        ++begin2;
    }
    if (begin1 == end1 && begin2 == end2)
    {
        return 0;
    }
    else if (begin1 == end1)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

} // namespace pw::internal
#endif /*  INCLUDED_PW_INTERNAL_COMPARE_H */
