#pragma once

/// @file
/// @brief Traits coming to C++42

#include <iterator>
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

/// @brief Combines std::remove_cv and std::remove_reference
/// @see https://en.cppreference.com/w/cpp/types/remove_cvref
///@{
template <class T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
///@}

/// @brief Obtains the value_type of iterator T
/// @see https://en.cppreference.com/w/cpp/iterator/iter_t
template <class T>
using iter_value_t =
    typename std::iterator_traits<remove_cvref_t<T>>::value_type;

}  // namespace spline::stdx
