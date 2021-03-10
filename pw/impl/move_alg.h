#ifndef INCLUDED_PW_IMPL_MOVE_ALG_H
#define INCLUDED_PW_IMPL_MOVE_ALG_H

#include <pw/impl/move.h>
#include <pw/impl/remove_reference.h>

namespace pw {

template<class InputIterator, class OutputIterator>
OutputIterator
move(InputIterator begin, InputIterator end, OutputIterator dest)
{
    while (begin != end)
    {
        *dest++ = pw::move(*begin++);
    }
    return dest;
}
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_MOVE_ALG_H */
