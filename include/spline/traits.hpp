#pragma once

/// @file
/// @brief Traits for the spline library

#include <type_traits>
#include "vector_space_algebra.hpp"

namespace spline {

namespace detail {

template <class V, class A, class = void>
struct has_vector_addition : std::false_type {};
template <class V, class A>
struct has_vector_addition<V, A,
                           std::void_t<decltype(std::declval<A>().add(
                               std::declval<V>(), std::declval<V>()))>>
    : std::bool_constant<
          std::is_same_v<V, decltype(std::declval<A>().add(
                                std::declval<V>(), std::declval<V>()))>> {};

template <class V, class A, class = void>
struct has_scalar_multiplication : std::false_type {};
template <class V, class A>
struct has_scalar_multiplication<
    V, A,
    std::void_t<decltype(std::declval<A>().mul(
        std::declval<typename A::scalar_type>(), std::declval<V>()))>>
    : std::bool_constant<
          std::is_same_v<V, decltype(std::declval<A>().mul(
                                std::declval<typename A::scalar_type>(),
                                std::declval<V>()))>> {};

}  // namespace detail

/// @brief Checks if the provided vector and vector space algebra types create a
/// valid vector space
/// @brief V Vector type
/// @brief A Vector space algebra type
///@{
template <class V, class A>
using is_vector_space =
    std::conjunction<algebra::is_vector_space_algebra<A>,
                     detail::has_vector_addition<V, A>,
                     detail::has_scalar_multiplication<V, A>>;

template <class V, class A>
inline constexpr bool is_vector_space_v = is_vector_space<V, A>::value;
///@}

}  // namespace spline
