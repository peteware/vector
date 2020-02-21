#ifndef INCLUDED_PW_IMPL_LEXICOGRAPHICAL_COMPARE_H
#define INCLUDED_PW_IMPL_LEXICOGRAPHICAL_COMPARE_H

namespace pw {

template<class Iterator1, class Iterator2>
bool
lexicographical_compare(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2)
{
    while (begin1 != end1 && begin2 != end2)
    {
        if (*begin1 < *begin2)
        {
            return true;
        }
        if (*begin2 < *begin1)
        {
            return false;
        }
        ++begin1;
        ++begin2;
    }
    return begin1 == end1 && begin2 != end2;
}

} // namespace pw
#endif /* INCLUDED_PW_IMPL_LEXICOGRAPHICAL_COMPARE_H */
