#ifndef INCLUDED_PW_IMPL_ALLOCATION_RESULT_H
#define INCLUDED_PW_IMPL_ALLOCATION_RESULT_H

namespace pw {

// Return type of allocator_traits::allocate_at_least (C++23).
// Models std::allocation_result<Pointer, SizeType>.
template<class Pointer, class SizeType>
struct allocation_result
{
    Pointer  ptr;
    SizeType count;
};

} // namespace pw
#endif /* INCLUDED_PW_IMPL_ALLOCATION_RESULT_H */