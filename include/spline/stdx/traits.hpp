#pragma once

/// @file
/// @brief Traits coming to C++42

#include <type_traits>

/// @namespace stdx
/// @brief std in the future
namespace spline::stdx {

/// @brief Checks if a type is a specialization of a template
/// @see https://wg21.link/p2098
///@{
template <class T, template <class...> class Primary>
struct is_specialization_of : std::false_type {};

template <template <class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template <class T, template <class...> class Primary>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<T, Primary>::value;
///@}

}  // namespace spline::stdx
