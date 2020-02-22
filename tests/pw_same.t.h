#ifndef INCLUDED_PW_SAME_T_H
#define INCLUDED_PW_SAME_T_H

namespace pw { namespace internal {
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
}} // namespace pw::internal
#endif /* INCLUDED_PW_SAME_T_H */
 
