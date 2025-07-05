#ifndef INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H
#define INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H

#include <pw/impl/addressof.h>
#include <pw/impl/construct_at.h>
#include <pw/impl/destroy.h>
#include <pw/impl/iterator_traits.h>
#include <pw/impl/move.h>

namespace pw {

template<class InputIterator, class OutputIterator>
void
uninitialized_move(InputIterator begin, InputIterator end, OutputIterator out)
{
    using Value = typename pw::iterator_traits<OutputIterator>::value_type;
    OutputIterator current = out;

    try
    {
        while (begin != end)
        {
            //::new (pw::addressof(*current)) Value(pw::move(*begin));

            construct_at(addressof(*current), pw::move(*begin));
            ++current;
            ++begin;
        }
    }
    catch (...)
    {
        pw::destroy(out, current);
        throw;
    }
}
} // namespace pw
#endif /*  INCLUDED_PW_IMPL_UNINITIALIZED_MOVE_CONSTRUCT_H */
