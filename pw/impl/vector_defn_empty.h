#ifndef INCLUDED_PW_IMPL_VECTOR_DEFN_H
#define INCLUDED_PW_IMPL_VECTOR_DEFN_H

#include <pw/impl/vector_decl.h>

namespace pw {
template<class Type>
Type&
makeReference()
{
    static Type s_value;

    return s_value;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector() noexcept(noexcept(allocator_type()))
{
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(allocator_type const& alloc) noexcept
{
    (void)alloc;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type             count,
                                          value_type const&     value,
                                          allocator_type const& alloc)
{
    (void)count;
    (void)value;
    (void)alloc;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type count, allocator_type const& alloc)
{
    (void)count;
    (void)alloc;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy)
{
    (void)copy;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy, allocator_type const& alloc)
{
    (void)copy;
    (void)alloc;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy) noexcept
{
    (void)copy;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy, const Allocator& alloc)
{
    (void)copy;
    (void)alloc;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(pw::initializer_list<value_type> init, allocator_type const& alloc)
{
    (void)init;
    (void)alloc;
}

template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::vector(Iterator first, Iterator last, allocator_type const& alloc)
{
    (void)first;
    (void)last;
    (void)alloc;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::swap(vector& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    (void)other;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(const vector& other)
{
    (void)other;
    return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(pw::initializer_list<value_type> init_list)
{
    (void)init_list;
    return *this;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(vector&& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    (void)other;
    return *this;
}

template<class Type, class Allocator>
template<class Iterator>
constexpr void
vector<Type, Allocator>::assign(Iterator begin, Iterator end)
{
    (void)begin;
    (void)end;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(size_type count, value_type const& value)
{
    (void)count;
    (void)value;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(pw::initializer_list<value_type> init_list)
{
    (void)init_list;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::allocator_type
vector<Type, Allocator>::get_allocator() const
{
    return Allocator();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::at(size_type position)
{
    (void)position;
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::at(size_type position) const
{
    (void)position;
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::operator[](size_type position)
{
    (void)position;
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::operator[](size_type position) const
{
    (void)position;
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::front()
{
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::front() const
{
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::back()
{
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::back() const
{
    return makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr Type*
vector<Type, Allocator>::data() noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr Type const*
vector<Type, Allocator>::data() const noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::begin() noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::begin() const noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::end() noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::end() const noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cbegin() const noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cend() const noexcept
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class Type, class Allocator>
constexpr bool
vector<Type, Allocator>::empty() const noexcept
{
    return true;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::size() const noexcept
{
    return 0;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::max_size() const noexcept
{
    return 1;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::capacity() const noexcept
{
    return 0;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::shrink_to_fit()
{
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::reserve(size_type count)
{
    (void)count;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::clear() noexcept
{
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(const_reference value)
{
    (void)value;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(value_type&& value)
{
    (void)value;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total)
{
    (void)total;
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total, const_reference value)
{
    (void)total;
    (void)value;
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator position)
{
    return erase(position, position + 1);
}

/**
 * Removes the elements in the range [begin, end).
 *
 * @param begin Iterator to the first element to remove
 * @param end   Iterator to one past the last element to remove
 *
 * @return Iterator following the last removed element.
 *         - If end == end() prior to removal, then the updated end() iterator is returned.
 *         - If [begin, end) is an empty range, then end is returned.
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::erase(const_iterator begin, const_iterator end)
{
    (void)begin;
    (void)end;
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::pop_back()
{
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, const_reference value)
{
    return insert(position, static_cast<size_type>(1), value);
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, value_type&& value)
{
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, size_type count, const_reference value)
{
    (void)position;
    (void)count;
    (void)value;
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, Iterator first, Iterator last)
{
    (void)position;
    (void)first;
    (void)last;
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, pw::initializer_list<value_type> init_list)
{
    (void)position;
    (void)init_list;
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    return makeReference<value_type>();
}

template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    (void)position;
    return &makeReference<value_type>();
}

template<class Type, class Allocator>
constexpr void
swap(vector<Type, Allocator>& op1, vector<Type, Allocator>& op2) noexcept(noexcept(op1.swap(op2)))
{
    (void)op1;
    (void)op2;
}

template<class Type, class Allocator>
constexpr bool
operator==(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
{
    if (op1.size() != op2.size())
    {
        return false;
    }
    return (op1 <=> op2) == 0;
}

template<class Type, class Allocator>
constexpr auto
operator<=>(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
    -> decltype(op1[0] <=> op2[0])
{
    return op1[0] <=> op2[0];
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_VECTOR_DEFN_H */
