#ifndef INCLUDED_PW_IMPL_BACK_INSERTER_H
#define INCLUDED_PW_IMPL_BACK_INSERTER_H

#include <pw/impl/back_insert_iterator.h>

namespace pw {

template<class Container>
back_insert_iterator<Container>
back_inserter(Container& container)
{
    return back_insert_iterator<Container>(container);
}

} // namespace pw
#endif /*  INCLUDED_PW_IMPL_BACK_INSERT_H */
