#pragma once

/// @file
/// @brief Type traits describing concepts in spline

#include <functional>
#include <type_traits>

/// @brief Spline concepts
namespace spline::concepts {
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
          class Mul = std::multiplies<>>
struct is_vector_space
    : std::conjunction<is_field<S>, is_vector_addition<V, Add>,
                       is_scalar_multiplication<S, V, Mul>> {};

template <class V, class S, class Add = std::plus<>,
          class Mul = std::multiplies<>>
inline constexpr bool is_vector_space_v =
    is_vector_space<V, S, Add, Mul>::value;
/// @}

}  // namespace spline::concepts
