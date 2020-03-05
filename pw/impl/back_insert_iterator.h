#ifndef INCLUDED_IMPL_BACK_INSERT_ITERATOR_H
#define INCLUDED_IMPL_BACK_INSERT_ITERATOR_H

#include <pw/impl/iterator_tag.h>

namespace pw {

class back_insert_iterator
{
public:
    using iterator_category = pw::output_iterator_tag;
    using value_type        = void;
    using difference_type   = void;
    using pointer           = void;
    using reference         = void;
    using container_type    = Container;

protected:
    container_type* m_container;
};

} // namespace pw

#endif /* INCLUDED_IMPL_BACK_INSERT_ITERATOR_H */
