#ifndef INCLUDED_PW_IMPL_ITERATOR_TAG_H
#define INCLUDED_PW_IMPL_ITERATOR_TAG_H

#include <iterator>

namespace pw {

/*
 * Originally, I just defined these structs.  I switched to
 * using the std version as when writing unit tests against
 * the std library it didn't recognized my iterator categories.
 */
using input_iterator_tag         = std::input_iterator_tag;
using output_iterator_tag        = std::output_iterator_tag;
using forward_iterator_tag       = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;

#ifdef notdef
struct input_iterator_tag
{
};

struct output_iterator_tag
{
};

struct forward_iterator_tag : public input_iterator_tag
{
};

struct bidirectional_iterator_tag : public forward_iterator_tag
{
};

struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};
#endif

} // namespace pw

#endif /*  INCLUDED_PW_IMPL_ITERATOR_TAG_H */
