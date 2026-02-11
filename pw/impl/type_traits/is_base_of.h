#ifndef INCLUDED_PW_IMPL_IS_BASE_OF_H
#define INCLUDED_PW_IMPL_IS_BASE_OF_H
#include <type_traits> // Used for std::is_base_of

namespace pw {
template<class Base, class Derived>
using is_base_of = std::is_base_of<Base, Derived>;
}

#endif //INCLUDED_PW_IMPL_IS_BASE_OF_H
