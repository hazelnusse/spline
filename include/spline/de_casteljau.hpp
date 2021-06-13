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

struct de_casteljau_fn final {
    /// @brief De Casteljau's algorithm in-place
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
inline constexpr detail::de_casteljau_fn de_casteljau{};
}  // namespace spline
