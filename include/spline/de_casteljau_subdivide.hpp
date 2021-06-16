#pragma once

/// @file
/// @brief De Casteljau's algorithm with subdivision

#include <functional>
#include <type_traits>
#include <utility>
#include "spline/concepts.hpp"
#include "spline/core/de_casteljau_subdivide.hpp"
#include "spline/traits.hpp"

/// @namespace spline
/// @brief The Spline library
namespace spline {
namespace detail {

/// @brief Function object providing a constrained form of
/// spline::core::de_casteljau_subdivide
struct de_casteljau_subdivide_fn final {
    /// @copydoc spline::core::de_casteljau_subdivide
    /// @note Requires:
    ///   - `std::is_base_of_v<std::input_iterator_tag,
    ///   xtd::iter_category_t<InputIt>>` is `true` and
    ///   - `std::is_base_of_v<std::bidirectional_iterator_tag,
    ///   xtd::iter_category_t<OutputIt>>` is `true` and
    ///   - `std::is_same_v<xtd::iter_value_t<InputIt>,
    ///   xtd::iter_value_t<OutputIt>>` is `true` and
    ///   - `concepts::is_vector_space_v<xtd::iter_category_t<InputIt>, Scalar,
    ///   Add, Mul>` is `true`
    ///
    /// For an unconstrained version of this algorithm, see
    /// spline::core::de_casteljau_subdivide.
    template <class InputIt, class OutputIt, class Scalar,
              class Mul = multiplies<Scalar, xtd::iter_value_t<InputIt>>,
              class Add = std::plus<>>
    constexpr auto operator()(InputIt first, InputIt last, OutputIt d_first,
                              Scalar t, Mul mul = {}, Add add = {}) const
        -> std::enable_if_t<
            std::conjunction_v<
                std::is_base_of<std::input_iterator_tag,
                                xtd::iter_category_t<InputIt>>,
                std::is_base_of<std::bidirectional_iterator_tag,
                                xtd::iter_category_t<OutputIt>>,
                std::is_same<xtd::iter_value_t<InputIt>,
                             xtd::iter_value_t<OutputIt>>,
                concepts::is_vector_space<xtd::iter_value_t<InputIt>, Scalar,
                                          Add, Mul>>,
            OutputIt>
    {
        return core::de_casteljau_subdivide(std::move(first), std::move(last),
                                            std::move(d_first), std::move(t),
                                            std::move(mul), std::move(add));
    }
};
}  // namespace detail

/// @copybrief core::de_casteljau_subdivide
/// @copydetails detail::de_casteljau_subdivide_fn::operator()()
/// @note This is implemented as global function object.
#ifdef DOXYGEN
template <class InputIt, class OutputIt, class Scalar,
          class Mul = multiplies<Scalar, xtd::iter_value_t<InputIt>>,
          class Add = std::plus<>>
constexpr auto de_casteljau_subdivide(InputIt first, InputIt last,
                                      OutputIt d_first, Scalar t, Mul mul = {},
                                      Add add = {}) -> OutputIt;
#else
inline constexpr detail::de_casteljau_subdivide_fn de_casteljau_subdivide{};
#endif
}  // namespace spline
