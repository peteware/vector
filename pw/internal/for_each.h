#ifndef INCLUDED_PW_INTERNAL_FOR_EACH_H
#define INCLUDED_PW_INTERNAL_FOR_EACH_H

namespace pw { namespace internal {

template<class InputIterator, class UnaryFunction>
UnaryFunction
for_each(InputIterator begin, InputIterator end, UnaryFunction function)
{
    while (begin != end)
    {
        function(*begin++);
    }
    return function;
}

}} // namespace pw::internal

#endif /* INCLUDED_PW_INTERNAL_FOR_EACH_H */
