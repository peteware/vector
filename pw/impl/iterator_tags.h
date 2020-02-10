#ifndef INCLUDED_PW_IMPL_ITERATOR_TAGS_H
#define INCLUDED_PW_IMPL_ITERATOR_TAGS_H

namespace pw {
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
struct contiguous_iterator_tag : public random_access_iterator_tag
{
};
} // namespace pw

#endif /*  INCLUDED_PW_IMPL_ITERATOR_TAGS_H */
