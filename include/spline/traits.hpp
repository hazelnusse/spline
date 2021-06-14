#pragma once

/// @file
/// @brief Type traits for the in spline

#include <iterator>
#include <type_traits>

/// @brief The C++ standard library in the future
namespace spline::xtd {

/// @brief Combines std::remove_cv and std::remove_reference
/// @see https://en.cppreference.com/w/cpp/types/remove_cvref
/// @{
template <class T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
/// @}

/// @brief Computes the associated value type of an iterator T
/// @see https://en.cppreference.com/w/cpp/iterator/iter_t
template <class T>
using iter_value_t =
    typename std::iterator_traits<remove_cvref_t<T>>::value_type;

template <class T>
using iter_category_t =
    typename std::iterator_traits<remove_cvref_t<T>>::iterator_category;

/// @brief Returns a boolean with dependent scope
/// @see https://wg21.link/p1830
template <bool Value, class... Args>
inline constexpr bool dependent_bool_v = Value;

}  // namespace spline::xtd
