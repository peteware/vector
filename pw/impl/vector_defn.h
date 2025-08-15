#ifndef INCLUDED_PW_IMPL_VECTOR_DEFN_H
#define INCLUDED_PW_IMPL_VECTOR_DEFN_H

#include <pw/impl/vector_decl.h>

#include <pw/impl/copy.h>
#include <pw/impl/destroy.h>
#include <pw/impl/distance.h>
#include <pw/impl/fill_n.h>
#include <pw/impl/is_base_of.h>
#include <pw/impl/min.h>
#include <pw/impl/move_alg.h>
#include <pw/impl/move_backward.h>
#include <pw/impl/uninitialized_copy.h>
#include <pw/impl/uninitialized_default_construct.h>
#include <pw/impl/uninitialized_fill.h>
#include <pw/impl/uninitialized_move.h>
#include <pw/internal/unimplemented.h>

namespace pw {

/**
 * @brief Default constructor. Constructs an empty vector.
 * @return A default-constructed vector with no elements
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector() noexcept(noexcept(allocator_type()))
    : m_storage(allocator_type())
{
}

/**
 * @brief Constructs an empty vector with the given allocator.
 * @param alloc The allocator to use for memory allocation
 * @return A vector with no elements using the specified allocator
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(allocator_type const& alloc) noexcept
    : m_storage(alloc)
{
}

/**
 * @brief Constructs a vector with count copies of value.
 * @param count The number of elements to create
 * @param value The value to initialize each element with
 * @param alloc The allocator to use for memory allocation
 * @return A vector containing count copies of value
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type             count,
                                          value_type const&     value,
                                          allocator_type const& alloc)
    : m_storage(alloc, count)
{
    m_storage.uninitialized_fill(m_storage.begin(), m_storage.begin() + count, value);
    m_storage.set_size(count);
}

/**
 * @brief Constructs a vector with count default-constructed elements.
 * @param count The number of elements to create
 * @param alloc The allocator to use for memory allocation
 * @return A vector containing count default-constructed elements
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(size_type count, allocator_type const& alloc)
    : m_storage(alloc, count)
{
    m_storage.uninitialized_default_construct(m_storage.begin(), m_storage.begin() + count);
    m_storage.set_size(count);
}

/**
 * @brief Copy constructor. Creates a copy of another vector.
 * @param copy The vector to copy from
 * @return A vector that is a copy of the given vector
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy)
    : m_storage(allocator_type(), copy.size())
{
    m_storage.uninitialized_copy(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

/**
 * @brief Copy constructor with allocator. Creates a copy using the specified allocator.
 * @param copy The vector to copy from
 * @param alloc The allocator to use for memory allocation
 * @return A vector that is a copy of the given vector using the specified allocator
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector const& copy, allocator_type const& alloc)
    : m_storage(alloc, copy.size())
{
    m_storage.uninitialized_copy(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

/**
 * @brief Move constructor. Takes ownership of another vector's resources.
 * @param copy The vector to move from
 * @return A vector that has taken ownership of the moved vector's resources
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy) noexcept
    : m_storage(allocator_type(), copy.size())
{
    m_storage.uninitialized_move(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

/**
 * @brief Move constructor with allocator. Moves from another vector using the specified allocator.
 * @param copy The vector to move from
 * @param alloc The allocator to use for memory allocation
 * @return A vector that has taken ownership of the moved vector's resources
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(vector&& copy, const Allocator& alloc)
    : m_storage(alloc, copy.size())
{
    m_storage.uninitialized_move(copy.begin(), copy.end(), m_storage.begin());
    m_storage.set_size(copy.size());
}

/**
 * @brief Constructs a vector from an initializer list.
 * @param init The initializer list to construct from
 * @param alloc The allocator to use for memory allocation
 * @return A vector containing the elements from the initializer list
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::vector(pw::initializer_list<value_type> init, allocator_type const& alloc)
    : m_storage(alloc, init.size())
{
    m_storage.uninitialized_copy(init.begin(), init.end(), m_storage.begin());
    m_storage.set_size(init.size());
}

/**
 * @brief Constructs a vector from an iterator range.
 * @param first Iterator to the first element to copy
 * @param last Iterator to one past the last element to copy
 * @param alloc The allocator to use for memory allocation
 * @return A vector containing the elements from the iterator range
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::vector(Iterator first, Iterator last, allocator_type const& alloc)
    : m_storage(alloc)
{
    if constexpr (pw::is_base_of<pw::forward_iterator_tag, Iterator>::value)
    {
        size_type count  = pw::distance(first, last);

        auto      lambda = [begin = first, end = last, this](pointer dest) -> void {
            m_storage.uninitialized_copy(begin, end, dest);
        };
        // TODO: Storage2.reserve() doesn't exist!  No unit test?
        m_storage.reserve(count, lambda);
    }
    else
    {
        // This is an input iterator so we need to grow as we go
        while (first != last)
        {
            push_back(*first);
            ++first;
        }
    }
}

/**
 * @brief Swaps the contents of this vector with another vector.
 * @param other The vector to swap with
 * @return None
 * @exception None (conditionally noexcept based on allocator traits)
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::swap(vector& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    m_storage.swap(other.m_storage, allocator_traits<allocator_type>::propagate_on_container_swap::value);
}

/**
 * @brief Copy assignment operator. Assigns contents from another vector.
 * @param other The vector to copy from
 * @return Reference to this vector
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(const vector& other)
{
    if constexpr (allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
    {
        // TODO: Check if same allocator.  If so, we should still
        //       copy the allocator however we do not need to reallocate
        //       the storage if there is enough space.
        Storage tmp { other.get_allocator(), other.size() };

        tmp.uninitialized_copy(other.begin(), other.end(), tmp.begin());
        tmp.set_size(other.size());
        m_storage.swap(tmp, true);
    }
    else if (m_storage.size() < other.size())
    {
        if (m_storage.allocated() >= other.size())
        {
            m_storage.copy(other.begin(), other.begin() + m_storage.size(), m_storage.begin());
            m_storage.uninitialized_copy(other.begin() + m_storage.size(), other.end(), m_storage.end());
        }
        else
        {
            // Not enough space so we need to reallocate
            Storage tmp { allocator_type(), other.size() };
            tmp.uninitialized_copy(other.begin(), other.end(), tmp.begin());
            tmp.set_size(other.size());
            m_storage.swap(tmp, false);
        }
        m_storage.set_size(other.size());
    }
    else if (m_storage.size() == other.size())
    {
        m_storage.copy(other.begin(), other.end(), m_storage.begin());
    }
    else // m_storage.size() > other.size()
    {
        m_storage.copy(other.begin(), other.end(), m_storage.begin());
        pw::destroy(m_storage.begin() + other.size(), m_storage.end());
        m_storage.set_size(other.size());
    }
    return *this;
}

/**
 * @brief Initializer list assignment operator. Assigns contents from an initializer list.
 * @param init_list The initializer list to assign from
 * @return Reference to this vector
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(pw::initializer_list<value_type> init_list)
{
    Storage tmp { allocator_type(), init_list.size() };

    tmp.uninitialized_copy(init_list.begin(), init_list.end(), tmp.begin());
    tmp.set_size(init_list.size());
    m_storage.swap(tmp, false);
    return *this;
}

/**
 * @brief Move assignment operator. Moves contents from another vector.
 * @param other The vector to move from
 * @return Reference to this vector
 * @exception None (conditionally noexcept based on allocator traits)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>&
vector<Type, Allocator>::operator=(vector&& other)
    noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
             pw::allocator_traits<allocator_type>::is_always_equal::value)
{
    if constexpr (allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)
    {
        Storage storage { other.get_allocator(), other.size() };
        storage.uninitialized_copy(other.begin(), other.end(), storage.begin());
        storage.set_size(other.size());
        m_storage.swap(storage, true);
    }
    else
    {
        size_type init_size = min(m_storage.size(), other.size());
        move(other.begin(), other.begin() + init_size, m_storage.begin());
        if (size() < other.size())
        {
            m_storage.uninitialized_move(
                other.begin() + init_size, other.end(), m_storage.begin() + init_size);
        }
        else
        {
            pw::destroy(m_storage.begin() + init_size, m_storage.end());
        }
        m_storage.set_size(other.size());
    }
    return *this;
}

/**
 * @brief Assigns new contents from an iterator range, replacing current contents.
 * @param begin Iterator to the first element to assign
 * @param end Iterator to one past the last element to assign
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
template<class Iterator>
constexpr void
vector<Type, Allocator>::assign(Iterator begin, Iterator end)
{
    if constexpr (is_base_of<pw::forward_iterator_tag, Iterator>::value)
    {
        size_type count = pw::distance(begin, end);
        Storage   tmp { allocator_type(), count };

        tmp.uninitialized_copy(begin, end, tmp.begin());
        tmp.set_size(count);
        m_storage.swap(tmp, false);
    }
    else
    {
        m_storage.reset();
        while (begin != end)
        {
            push_back(*begin);
            ++begin;
        }
    }
}

/**
 * @brief Assigns count copies of value, replacing current contents.
 * @param count The number of elements to assign
 * @param value The value to assign to each element
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(size_type count, value_type const& value)
{
    Storage tmp { allocator_type(), count };
    tmp.uninitialized_fill(tmp.begin(), tmp.begin() + count, value);
    tmp.set_size(count);
    m_storage.swap(tmp, false);
}

/**
 * @brief Assigns contents from an initializer list, replacing current contents.
 * @param init_list The initializer list to assign from
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::assign(pw::initializer_list<value_type> init_list)
{
    Storage tmp { allocator_type(), init_list.size() };
    tmp.uninitialized_copy(init_list.begin(), init_list.end(), tmp.begin());
    tmp.set_size(init_list.size());
    m_storage.swap(tmp, false);
}

/**
 * @brief Returns the allocator associated with the vector.
 * @return Copy of the allocator
 * @exception None
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::allocator_type
vector<Type, Allocator>::get_allocator() const
{
    return m_storage.copy_allocator();
}

/**
 * @brief Returns a reference to the element at specified position with bounds checking.
 * @param position The position of the element to return
 * @return Reference to the requested element
 * @exception std::out_of_range if position >= size()
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::at(size_type position)
{
    if (position >= size())
    {
        throw std::out_of_range("vector::at");
    }
    return *(m_storage.begin() + position);
}

/**
 * @brief Returns a const reference to the element at specified position with bounds checking.
 * @param position The position of the element to return
 * @return Const reference to the requested element
 * @exception std::out_of_range if position >= size()
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::at(size_type position) const
{
    if (position >= size())
    {
        throw std::out_of_range("vector::at");
    }
    return *(m_storage.begin() + position);
}

/**
 * @brief Returns a reference to the element at specified position (no bounds checking).
 * @param position The position of the element to return
 * @return Reference to the requested element
 * @exception None (undefined behavior if position >= size())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::operator[](size_type position)
{
    return *(m_storage.begin() + position);
}

/**
 * @brief Returns a const reference to the element at specified position (no bounds checking).
 * @param position The position of the element to return
 * @return Const reference to the requested element
 * @exception None (undefined behavior if position >= size())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::operator[](size_type position) const
{
    return *(m_storage.begin() + position);
}

/**
 * @brief Returns a reference to the first element.
 * @return Reference to the first element
 * @exception None (undefined behavior if empty())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::front()
{
    return *m_storage.begin();
}

/**
 * @brief Returns a const reference to the first element.
 * @return Const reference to the first element
 * @exception None (undefined behavior if empty())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::front() const
{
    return *m_storage.begin();
}

/**
 * @brief Returns a reference to the last element.
 * @return Reference to the last element
 * @exception None (undefined behavior if empty())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::back()
{
    return *(m_storage.end() - 1);
}

/**
 * @brief Returns a const reference to the last element.
 * @return Const reference to the last element
 * @exception None (undefined behavior if empty())
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reference
vector<Type, Allocator>::back() const
{
    return *(m_storage.end() - 1);
}

/**
 * @brief Returns a pointer to the underlying array.
 * @return Pointer to the underlying element storage, or nullptr if empty
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr Type*
vector<Type, Allocator>::data() noexcept
{
    if (empty())
        return nullptr;
    return m_storage.begin();
}

/**
 * @brief Returns a const pointer to the underlying array.
 * @return Const pointer to the underlying element storage, or nullptr if empty
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr Type const*
vector<Type, Allocator>::data() const noexcept
{
    if (empty())
        return nullptr;
    return m_storage.begin();
}

/**
 * @brief Returns an iterator to the beginning.
 * @return Iterator to the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::begin() noexcept
{
    return m_storage.begin();
}

/**
 * @brief Returns a const iterator to the beginning.
 * @return Const iterator to the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::begin() const noexcept
{
    return m_storage.begin();
}

/**
 * @brief Returns an iterator to the end.
 * @return Iterator to one past the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::end() noexcept
{
    return m_storage.end();
}

/**
 * @brief Returns a const iterator to the end.
 * @return Const iterator to one past the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::end() const noexcept
{
    return m_storage.end();
}

/**
 * @brief Returns a const iterator to the beginning.
 * @return Const iterator to the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cbegin() const noexcept
{
    return m_storage.begin();
}

/**
 * @brief Returns a const iterator to the end.
 * @return Const iterator to one past the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_iterator
vector<Type, Allocator>::cend() const noexcept
{
    return m_storage.end();
}

/**
 * @brief Returns a reverse iterator to the beginning.
 * @return Reverse iterator to the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

/**
 * @brief Returns a const reverse iterator to the beginning.
 * @return Const reverse iterator to the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

/**
 * @brief Returns a reverse iterator to the end.
 * @return Reverse iterator to one before the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::reverse_iterator
vector<Type, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

/**
 * @brief Returns a const reverse iterator to the end.
 * @return Const reverse iterator to one before the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

/**
 * @brief Returns a const reverse iterator to the beginning.
 * @return Const reverse iterator to the last element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

/**
 * @brief Returns a const reverse iterator to the end.
 * @return Const reverse iterator to one before the first element
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::const_reverse_iterator
vector<Type, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

/**
 * @brief Checks whether the vector is empty.
 * @return true if the vector is empty, false otherwise
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr bool
vector<Type, Allocator>::empty() const noexcept
{
    return m_storage.empty();
}

/**
 * @brief Returns the number of elements.
 * @return The number of elements in the vector
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::size() const noexcept
{
    return m_storage.size();
}

/**
 * @brief Returns the maximum number of elements.
 * @return The maximum possible number of elements
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
// ReSharper disable once CppMemberFunctionMayBeStatic
vector<Type, Allocator>::max_size() const noexcept
{
    return (static_cast<size_type>(1) << (sizeof(size_type) * 8 - 4)) / sizeof(value_type);
}

/**
 * @brief Returns the number of elements that can be held in currently allocated storage.
 * @return The capacity of the currently allocated storage
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::size_type
vector<Type, Allocator>::capacity() const noexcept
{
    return m_storage.allocated();
}

/**
 * @brief Reduces memory usage by freeing unused memory.
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::shrink_to_fit()
{
    if (m_storage.allocated() == m_storage.size())
    {
        return;
    }
    Storage tmp(m_storage.copy_allocator(), m_storage.size());
    tmp.uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
    tmp.set_size(m_storage.size());
    m_storage.swap(tmp, false);
}

/**
 * @brief Reserves storage for at least count elements.
 * @param count The number of elements to reserve capacity for
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::reserve(size_type count)
{
    if (count <= m_storage.allocated())
        return;
    Storage tmp(m_storage.copy_allocator(), count);
    tmp.uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
    tmp.set_size(m_storage.size());
    m_storage.swap(tmp, false);
}

/**
 * @brief Removes all elements from the vector.
 * @return None
 * @exception None (noexcept)
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::clear() noexcept
{
    // Calling clear() does not affect the result of capacity().
    pw::destroy(m_storage.begin(), m_storage.end());
    m_storage.set_size(0);
}

/**
 * @brief Adds an element to the end.
 * @param value The value to append
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(const_reference value)
{
    constexpr size_type count = 1;
    size_type const     total = m_storage.size() + count;

    if (total <= m_storage.allocated())
    {
        // TODO: Change calls to allocator_traits<>::construct() to Storage2::construct()
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*m_storage.end()), value);
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.calc_size());

        tmp.uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(tmp.begin() + total - count)), value);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

/**
 * @brief Adds an element to the end by moving.
 * @param value The value to move and append
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::push_back(value_type&& value)
{
    constexpr size_type count = 1;
    size_type const     total = m_storage.size() + count;

    if (total <= m_storage.allocated())
    {
        // TODO: Change calls to construct_at() to Storage2::construct()
        pw::construct_at(pw::addressof(*m_storage.end()), pw::move(value));
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.calc_size());

        tmp.uninitialized_move(m_storage.begin(), m_storage.end(), tmp.begin());
        pw::allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(tmp.begin() + total - 1)), pw::move(value));
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

/**
 * @brief Resizes the vector to contain total elements.
 * @param total The new size of the vector
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total)
{
    if (total == 0)
    {
        clear();
    }
    else if (total <= size())
    {
        // TODO: Change calls to destroy() to Storage2::destroy()
        pw::destroy(m_storage.begin() + total, m_storage.end());
    }
    else if (total <= m_storage.allocated())
    {
        m_storage.uninitialized_default_construct(m_storage.end(), m_storage.end() + total - size());
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), total);

        tmp.uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        tmp.uninitialized_default_construct(tmp.begin() + m_storage.size(), tmp.begin() + total);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

/**
 * @brief Resizes the vector to contain total elements with specified value.
 * @param total The new size of the vector
 * @param value The value to initialize new elements with
 * @return None
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::resize(size_type total, const_reference value)
{
    if (total == 0)
    {
        clear();
    }
    else if (total <= size())
    {
        pw::destroy(m_storage.begin() + total, m_storage.end());
    }
    else if (total <= m_storage.allocated())
    {
        m_storage.uninitialized_fill(m_storage.end(), m_storage.end() + total - size(), value);
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), total);

        tmp.uninitialized_copy(m_storage.begin(), m_storage.end(), tmp.begin());
        tmp.uninitialized_fill(tmp.begin() + m_storage.size(), tmp.begin() + total, value);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
}

/**
 * @brief Removes the element at the given position.
 * @param position Iterator to the element to remove
 * @return Iterator following the last removed element
 * @exception None
 */
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
    size_type const offset = pw::distance(cbegin(), begin);
    size_type const last   = pw::distance(cbegin(), end);
    size_type const width  = pw::distance(begin, end);

    // TODO: Replace move() and destroy() with Storage2::move(), Storage2::destroy()
    pw::move(m_storage.begin() + last, m_storage.end(), m_storage.begin() + offset);
    pw::destroy(m_storage.begin() + last, m_storage.end());
    m_storage.set_size(size() - width);
    return m_storage.begin() + offset;
}

/**
 * @brief Removes the last element.
 * @return None
 * @exception None (undefined behavior if empty())
 */
template<class Type, class Allocator>
constexpr void
vector<Type, Allocator>::pop_back()
{
    (void)erase(end() - 1);
}

/**
 * @brief Inserts a value before the given position.
 * @param position Iterator before which the content will be inserted
 * @param value The value to insert
 * @return Iterator pointing to the inserted value
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, const_reference value)
{
    return insert(position, static_cast<size_type>(1), value);
}

/**
 * @brief Inserts a value by moving before the given position.
 * @param position Iterator before which the content will be inserted
 * @param value The value to move and insert
 * @return Iterator pointing to the inserted value
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, value_type&& value)
{
    constexpr size_type count  = 1;
    size_type const     offset = pw::distance(cbegin(), position);
    size_type const     total  = size() + count;

    if (total <= m_storage.allocated())
    {
        if (position == cend())
        {
            m_storage.uninitialized_fill(m_storage.end(), m_storage.end() + count, pw::move(value));
        }
        else
        {
            // TODO: replace move_backward(), fill_n() with Storage2::move_backward(), Storage2::fill_n()
            m_storage.uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
            pw::move_backward(m_storage.begin() + offset, m_storage.end() - count, m_storage.end());
            pw::fill_n(m_storage.begin() + offset, count, value);
        }
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.calc_size());

        tmp.uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        pw::allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + offset)), value);
        tmp.uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

/**
 * @brief Inserts count copies of value before the given position.
 * @param position Iterator before which the content will be inserted
 * @param count Number of elements to insert
 * @param value The value to insert
 * @return Iterator pointing to the first inserted element
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, size_type count, const_reference value)
{
    size_type const offset = pw::distance(cbegin(), position);
    size_type const total  = size() + count;

    if (total < m_storage.allocated())
    {
        if (position == cend())
        {
            m_storage.uninitialized_fill(m_storage.end(), m_storage.end() + count, value);
        }
        else
        {
            m_storage.uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
            pw::move_backward(m_storage.begin() + offset, m_storage.end() - count, m_storage.end());
            // TODO: Can this fill to uninitialized memory?
            pw::fill_n(m_storage.begin() + offset, count, value);
        }
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), total);

        tmp.uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        tmp.uninitialized_fill(tmp.begin() + offset, tmp.begin() + offset + count, value);
        tmp.uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

/**
 * @brief Inserts elements from range [first, last) before position.
 * @param position Iterator before which the content will be inserted
 * @param first Iterator to the first element to insert
 * @param last Iterator to one past the last element to insert
 * @return Iterator pointing to the first inserted element
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
template<class Iterator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, Iterator first, Iterator last)
{
    size_type offset = pw::distance(cbegin(), position);
    iterator  dest   = m_storage.begin() + offset;

    while (first != last)
    {
        dest = insert(dest, *first) + 1;
        ++first;
    }
    return m_storage.begin() + offset;
}

/**
 * @brief Inserts elements from initializer list before position.
 * @param position Iterator before which the content will be inserted
 * @param init_list Initializer list to insert the values from
 * @return Iterator pointing to the first inserted element
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::insert(const_iterator position, pw::initializer_list<value_type> init_list)
{
    size_type       offset = pw::distance(cbegin(), position);
    size_type const total  = size() + init_list.size();

    if (total <= m_storage.allocated())
    {
        m_storage.uninitialized_move(m_storage.end() - init_list.size(), m_storage.end(), m_storage.end());
        pw::move_backward(m_storage.begin() + offset, m_storage.end() - init_list.size(), m_storage.end());
        pw::move(init_list.begin(), init_list.end(), m_storage.begin() + offset);
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.size() + init_list.size());

        tmp.uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        tmp.uninitialized_move(init_list.begin(), init_list.end(), tmp.begin() + offset);
        if (!m_storage.empty())
        {
            tmp.uninitialized_move(
                m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + init_list.size());
        }
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

/**
 * @brief Constructs and inserts an element at the end.
 * @param args Arguments to forward to the constructor of the element
 * @return Reference to the inserted element
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::reference
vector<Type, Allocator>::emplace_back(Args&&... args)
{
    size_type const total = size() + 1;
    if (total <= capacity())
    {
        allocator_traits<Allocator>::construct(m_storage.allocator(),
                                               pw::addressof(*(m_storage.begin() + total - 1)),
                                               pw::forward<Args>(args)...);
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.calc_size());

        tmp.uninitialized_move(m_storage.begin(), m_storage.end(), tmp.begin());
        allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + total - 1)), pw::forward<Args>(args)...);
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return back();
}

/**
 * @brief Constructs and inserts an element at the specified position.
 * @param position Iterator before which the new element will be constructed
 * @param args Arguments to forward to the constructor of the element
 * @return Iterator pointing to the emplaced element
 * @exception std::bad_alloc if memory allocation fails
 */
template<class Type, class Allocator>
template<class... Args>
constexpr vector<Type, Allocator>::iterator
vector<Type, Allocator>::emplace(const_iterator position, Args&&... args)
{
    size_type const     total  = size() + 1;
    constexpr size_type count  = 1;
    size_type const     offset = pw::distance(cbegin(), position);
    iterator            where  = m_storage.begin() + offset;

    if (total <= capacity())
    {
        m_storage.uninitialized_move(m_storage.end() - count, m_storage.end(), m_storage.end());
        pw::move_backward(m_storage.begin() + offset, m_storage.end(), m_storage.end() - count);
        allocator_traits<Allocator>::construct(
            m_storage.allocator(), pw::addressof(*(m_storage.begin() + offset)), pw::forward<Args>(args)...);
    }
    else
    {
        Storage tmp(m_storage.copy_allocator(), m_storage.calc_size());

        tmp.uninitialized_move(m_storage.begin() + offset, m_storage.end(), tmp.begin() + offset + count);
        pw::allocator_traits<Allocator>::construct(
            tmp.allocator(), pw::addressof(*(tmp.begin() + offset)), pw::forward<Args>(args)...);
        tmp.uninitialized_move(m_storage.begin(), m_storage.begin() + offset, tmp.begin());
        m_storage.swap(tmp, false);
    }
    m_storage.set_size(total);
    return m_storage.begin() + offset;
}

/**
 * @brief Specializes the swap algorithm for vector.
 * @param op1 First vector to swap
 * @param op2 Second vector to swap
 * @return None
 * @exception None (conditionally noexcept based on vector::swap)
 */
template<class Type, class Allocator>
constexpr void
swap(vector<Type, Allocator>& op1, vector<Type, Allocator>& op2) noexcept(noexcept(op1.swap(op2)))
{
    op1.swap(op2);
}

/**
 * @brief Checks if the contents of two vectors are equal.
 * @param op1 First vector to compare
 * @param op2 Second vector to compare
 * @return true if the contents are equal, false otherwise
 * @exception None
 */
template<class Type, class Allocator>
constexpr bool
operator==(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
{
    if (op1.size() != op2.size())
    {
        return false;
    }
    return op1 <=> op2 == 0;
}

/**
 * @brief Compares the contents of two vectors lexicographically.
 * @param op1 First vector to compare
 * @param op2 Second vector to compare
 * @return Strong ordering result of the comparison
 * @exception None
 */
template<class Type, class Allocator>
constexpr auto
operator<=>(const pw::vector<Type, Allocator>& op1, const pw::vector<Type, Allocator>& op2)
    -> decltype(op1[0] <=> op2[0])
{
    for (size_t i = 0; i < pw::min(op1.size(), op2.size()); ++i)
    {
        auto cmp = op1[i] <=> op2[i];
        if (cmp != 0)
        {
            return cmp;
        }
    }
    if (op1.size() < op2.size())
    {
        return std::strong_ordering::less;
    }
    else if (op1.size() > op2.size())
    {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
}
} // namespace pw
#endif /* INCLUDED_PW_IMPL_VECTOR_DEFN_H */
