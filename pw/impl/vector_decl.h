#ifndef PW_IMPL_VECTOR_DECL_H // -*- c++ -*-
#define PW_IMPL_VECTOR_DECL_H

#include <pw/impl/allocator.h>
#include <pw/impl/allocator_traits.h>
#include <pw/impl/initializer_list.h>
#include <pw/impl/iterator_traits.h>
#include <pw/impl/reverse_iterator.h>

#include <pw/internal/storage2.h>

namespace pw {

template<class Type, class Allocator = allocator<Type>>
class vector
{
public:
    using value_type             = Type;
    using allocator_type         = Allocator;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = typename allocator_traits<Allocator>::pointer;
    using const_pointer          = typename allocator_traits<Allocator>::const_pointer;
    using iterator               = typename allocator_traits<Allocator>::pointer;
    using const_iterator         = typename allocator_traits<Allocator>::const_pointer;
    using reverse_iterator       = pw::reverse_iterator<iterator>;
    using const_reverse_iterator = pw::reverse_iterator<iterator const>;

    constexpr vector() noexcept(noexcept(allocator_type()));
    constexpr explicit vector(allocator_type const& alloc) noexcept;
    constexpr vector(size_type             count,
                     value_type const&     value,
                     allocator_type const& alloc = allocator_type());
    constexpr explicit vector(size_type count, allocator_type const& alloc = allocator_type());
    constexpr vector(vector const& other);
    constexpr vector(vector const& other, allocator_type const& alloc);
    constexpr vector(vector&& other) noexcept;
    constexpr vector(vector&& other, allocator_type const& alloc);
    constexpr vector(pw::initializer_list<value_type> init, allocator_type const& alloc = allocator_type());

    template<class Iterator>
    constexpr vector(Iterator first, Iterator last, allocator_type const& alloc = allocator_type());

    constexpr ~vector();

    constexpr vector& operator=(const vector& other);
    constexpr vector& operator=(pw::initializer_list<value_type> init_list);
    constexpr vector& operator=(vector&& other) noexcept(
        pw::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
        pw::allocator_traits<allocator_type>::is_always_equal::value);

    constexpr void
    swap(vector& other) noexcept(pw::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                                 pw::allocator_traits<allocator_type>::is_always_equal::value);

    template<class Iterator>
    constexpr void                    assign(Iterator begin, Iterator end);
    constexpr void                    assign(size_type count, value_type const& value);
    constexpr void                    assign(pw::initializer_list<value_type> ilist);
    constexpr allocator_type          get_allocator() const;
    constexpr reference               at(size_type position);
    constexpr const_reference         at(size_type position) const;
    constexpr reference               operator[](size_type position);
    constexpr const_reference         operator[](size_type position) const;
    constexpr reference               front();
    constexpr const_reference         front() const;
    constexpr reference               back();
    constexpr const_reference         back() const;
    constexpr value_type*             data() noexcept;
    constexpr value_type const*       data() const noexcept;
    constexpr iterator                begin() noexcept;
    constexpr iterator                end() noexcept;
    constexpr const_iterator          begin() const noexcept;
    constexpr const_iterator          end() const noexcept;
    constexpr const_iterator          cbegin() const noexcept;
    constexpr const_iterator          cend() const noexcept;
    constexpr reverse_iterator        rbegin() noexcept;
    constexpr reverse_iterator        rend() noexcept;
    constexpr const_reverse_iterator  rbegin() const noexcept;
    constexpr const_reverse_iterator  rend() const noexcept;
    constexpr const_reverse_iterator  crbegin() const noexcept;
    constexpr const_reverse_iterator  crend() const noexcept;
    [[nodiscard]] constexpr bool      empty() const noexcept;
    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;
    constexpr void                    shrink_to_fit();
    constexpr void                    reserve(size_type count);
    constexpr void                    clear() noexcept;
    constexpr void                    push_back(const_reference value);
    constexpr void                    push_back(value_type&& value);
    constexpr void                    resize(size_type count);
    constexpr void                    resize(size_type count, const_reference value);
    constexpr iterator                erase(const_iterator position);
    constexpr iterator                erase(const_iterator begin, const_iterator end);
    constexpr void                    pop_back();
    constexpr iterator                insert(const_iterator position, const_reference value);
    constexpr iterator                insert(const_iterator position, value_type&& value);
    constexpr iterator                insert(const_iterator position, size_type count, const_reference value);
    constexpr iterator                insert(const_iterator position, pw::initializer_list<value_type> ilist);
    template<class Iterator>
    constexpr iterator insert(const_iterator position, Iterator first, Iterator last);
    template<class... Args>
    constexpr reference emplace_back(Args&&... args);
    template<class... Args>
    constexpr iterator emplace(const_iterator position, Args&&... args);

private:
    using Storage = internal::Storage2<Type, Allocator>;

    Storage m_storage;
};

template<class Type, class Allocator>
void constexpr swap(vector<Type, Allocator>& op1,
                    vector<Type, Allocator>& op2) noexcept(noexcept(op1.swap(op2)));

template<class Type, class Allocator>
constexpr bool operator==(const vector<Type, Allocator>& op1, const vector<Type, Allocator>& op2);

template<class Type, class Allocator>
constexpr auto operator<=>(const vector<Type, Allocator>& op1, const vector<Type, Allocator>& op2)
    -> decltype(op1[0] <=> op2[0]);

} // namespace pw
#endif /* PW_IMPL_VECTOR_DECL_H */
