#pragma once

/// @file
/// @brief Type traits describing concepts in spline

#include <functional>
#include <type_traits>
#include <utility>
#include "spline/traits.hpp"

namespace spline {

namespace multiplies_detail {

struct fallback {};

/// @brief Fallback overload of `operator*` allowing `multiplies` to be SFINAE
///     friendly
///
/// Provides an overload that always matches to `operator*` with a return type
/// of `fallback`. Used to determine if all other function in the overload set
/// are not candidates.
template <class T, class U>
constexpr auto operator*(const T&, const U&) -> fallback;

/// @brief Function object for performing multiplication
/// @tparam Left Left operand
/// @tparam Right Right operand
template <class Left, class Right>
struct multiplies {
    template <class L, class R>
    constexpr auto operator()(L&& lhs, R&& rhs) const -> std::enable_if_t<
        std::conjunction_v<std::is_same<Left, xtd::remove_cvref_t<L>>,
                           std::is_same<Right, xtd::remove_cvref_t<R>>>,
        decltype(std::forward<L>(lhs) * std::forward<R>(rhs))>
    {
        return std::forward<L>(lhs) * std::forward<R>(rhs);
    }
};

}  // namespace multiplies_detail

using multiplies_detail::multiplies;

/// @brief Spline concepts
namespace concepts {

/// @brief Determines if a type S and provided function object types define a
/// field
/// @tparam S Scalar type
/// @tparam Add Scalar addition function object type
/// @tparam Sub Scalar subtraction function object type
/// @tparam Mul Scalar multiplication function object type
/// @tparam Div Scalar division function object type
/// @tparam Neg Scalar negation function object type
/// @{
template <class S, class Add = std::plus<>, class Sub = std::minus<>,
          class Mul = std::multiplies<>, class Div = std::divides<>,
          class Neg = std::negate<>>
struct is_field
    : std::conjunction<std::is_invocable_r<S, const Add&, const S&, const S&>,
                       std::is_invocable_r<S, const Sub&, const S&, const S&>,
                       std::is_invocable_r<S, const Mul&, const S&, const S&>,
                       std::is_invocable_r<S, const Div&, const S&, const S&>,
                       std::is_invocable_r<S, const Neg&, const S&>> {};

template <class S, class Add = std::plus<>, class Sub = std::minus<>,
          class Mul = std::multiplies<>, class Div = std::divides<>,
          class Neg = std::negate<>>
inline constexpr bool is_field_v = is_field<S, Add, Sub, Mul, Div, Neg>::value;
/// @}

/// @brief Determines if a function object type implements vector addition
/// on a vector space
/// @tparam V Vector type
/// @tparam Add Vector addition function object type
template <class V, class Add>
struct is_vector_addition
    : std::is_invocable_r<V, const Add&, const V&, const V&> {};

/// @brief Determines if a function object type implements scalar
/// multiplication on a vector space
/// @tparam S Scalar type
/// @tparam V Vector type
/// @tparam Mul Scalar multiplication function object type
///
/// Determine if Mul performs left scalar multiplication:
///   S * V -> V
template <class S, class V, class Mul>
struct is_scalar_multiplication
    : std::is_invocable_r<V, const Mul&, const S&, const V&> {};

/// @brief Determines if the provided addition operation and multiplication
///     operation types form a vector space
/// @tparam V Vector type
/// @tparam S Scalar type
/// @tparam Add Vector addition function object type
/// @tparam Mul Scalar multiplication function object type
/// @{
template <class V, class S, class Add = std::plus<>,
          class Mul = multiplies<S, V>>
struct is_vector_space
    : std::conjunction<is_field<S>, is_vector_addition<V, Add>,
                       is_scalar_multiplication<S, V, Mul>> {};

template <class V, class S, class Add = std::plus<>,
          class Mul = multiplies<S, V>>
inline constexpr bool is_vector_space_v =
    is_vector_space<V, S, Add, Mul>::value;
/// @}

}  // namespace concepts
}  // namespace spline
