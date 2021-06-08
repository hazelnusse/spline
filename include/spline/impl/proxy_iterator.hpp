#pragma once

/// @file
/// @brief An "iterator" that converts between an algebraic_vector and its
/// underlying type

#include <iterator>
#include "algebraic_vector.hpp"

namespace spline::algebra::impl {

template <class Iterator, class Algebra>
struct proxy_iterator : Algebra {  // FIXME? don't inherit due to assignment
    /// This should be input iterator since we don't return a real reference
    using iterator_category =
        typename std::iterator_traits<Iterator>::iterator_category;
    using difference_type =
        typename std::iterator_traits<Iterator>::difference_type;

    using underlying_type = typename std::iterator_traits<Iterator>::value_type;
    using algebraic_vector_type = algebraic_vector<underlying_type, Algebra>;

    using value_type = algebraic_vector_type;
    using pointer = void;

    struct reference {
        reference& operator=(algebraic_vector_type v)
        {
            value = std::move(v.value);
            return *this;
        }

        constexpr operator algebraic_vector_type() const
        {
            return {value, algebra};
        }

        typename std::iterator_traits<Iterator>::reference value;
        Algebra algebra;
    };

    constexpr proxy_iterator(Iterator it, Algebra a) :
        Algebra{std::move(a)}, iter{std::move(it)}
    {}

    constexpr reference operator*() const { return reference{*iter, *this}; }

    constexpr proxy_iterator& operator++()
    {
        ++iter;
        return *this;
    }
    constexpr proxy_iterator operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    constexpr proxy_iterator& operator--()
    {
        --iter;
        return *this;
    }
    constexpr proxy_iterator operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }

    constexpr proxy_iterator& operator+=(difference_type n)
    {
        iter += n;
        return *this;
    }
    constexpr proxy_iterator& operator-=(difference_type n)
    {
        return (*this) += -n;
    }

    friend constexpr proxy_iterator operator+(difference_type n,
                                              const proxy_iterator& a)
    {
        auto temp = a;
        return temp += n;
    }
    friend constexpr proxy_iterator operator+(const proxy_iterator& a,
                                              difference_type n)
    {
        return n + a;
    }
    friend constexpr proxy_iterator operator-(const proxy_iterator& i,
                                              difference_type n)
    {
        return i + -n;
    }

    friend constexpr difference_type operator-(const proxy_iterator& b,
                                               const proxy_iterator& a)
    {
        return b.iter - a.iter;
    }

    constexpr reference operator[](difference_type n) const;

    friend constexpr bool operator==(const proxy_iterator& a,
                                     const proxy_iterator& b)
    {
        return a.iter == b.iter;
    };
    friend constexpr bool operator!=(const proxy_iterator& a,
                                     const proxy_iterator& b)
    {
        return !(a == b);
    }
    friend constexpr bool operator<(const proxy_iterator& a,
                                    const proxy_iterator& b)
    {
        return a.iter < b.iter;
    };
    friend constexpr bool operator>(const proxy_iterator& a,
                                    const proxy_iterator& b)
    {
        return b < a;
    };
    friend constexpr bool operator<=(const proxy_iterator& a,
                                     const proxy_iterator& b)
    {
        return !(a > b);
    };
    friend constexpr bool operator>=(const proxy_iterator& a,
                                     const proxy_iterator& b)
    {
        return !(a < b);
    };

    Iterator iter;
};

}  // namespace spline::algebra::impl
