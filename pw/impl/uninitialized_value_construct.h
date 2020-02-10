#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H

namespace pw {
template<class Iterator>
void
uninitialized_value_construct(Iterator begin, Iterator end)
{
    using Value      = typename pw::iterator_traits<Iterator>::value_type;
    Iterator current = begin;
    try
    {
        while (current != end)
        {
            ::new (static_cast<void*>(pw::addressof(*current))) Value();
            ++current;
        }
    }
    catch (...)
    {
        pw::destroy(begin, current);
        throw;
    }
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H */
