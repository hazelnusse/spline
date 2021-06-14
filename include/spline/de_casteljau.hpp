#pragma once

/// @file
/// @brief De Casteljau's algorithm without subdivision

#include <functional>
#include <type_traits>
#include <utility>
#include "spline/concepts.hpp"
#include "spline/core/de_casteljau.hpp"
#include "spline/traits.hpp"

namespace spline {
namespace detail {

/// @brief Function object providing a constrained form of core::de_casteljau()
struct de_casteljau_fn final {
    /// @copydoc core::de_casteljau
    /// @note Requires:
    ///   - `std::is_base_of_v<std::bidirectional_iterator_tag,
    ///   xtd::iter_category_t<InputIt>>` is `true` and
    ///   - `concepts::is_vector_space_v<xtd::iter_category_t<InputIt>, Scalar,
    ///   Add, Mul>` is `true`
    template <class InputIt, class Scalar,
              class Mul = multiplies<Scalar, xtd::iter_value_t<InputIt>>,
              class Add = std::plus<>>
    constexpr auto operator()(InputIt first, InputIt last, Scalar t,
                              Mul mul = {}, Add add = {}) const
        -> std::enable_if_t<
            std::conjunction_v<
                std::is_base_of<std::bidirectional_iterator_tag,
                                xtd::iter_category_t<InputIt>>,
                concepts::is_vector_space<xtd::iter_value_t<InputIt>, Scalar,
                                          Add, Mul>>,
            InputIt>
    {
        return core::de_casteljau(std::move(first), std::move(last),
                                  std::move(t), std::move(mul), std::move(add));
    }
};
}  // namespace detail

/// @brief Global function object performing De Casteljau's algorithm
/// @see detail::de_casteljau_fn
inline constexpr detail::de_casteljau_fn de_casteljau{};
}  // namespace spline
