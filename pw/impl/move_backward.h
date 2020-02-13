#ifndef INCLUDED_PW_IMPL_MOVE_BACKWARD_H
#define INCLUDED_PW_IMPL_MOVE_BACKWARD_H

#include <pw/impl/move.h>

namespace pw {

template<class Iterator1, class Iterator2>
Iterator2
move_backward(Iterator1 begin, Iterator1 end, Iterator2 dest)
{
    while (begin != end)
    {
        *(--dest) = pw::move(*(--end));
    }
    return dest;
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_MOVE_BACKWARD_H */
