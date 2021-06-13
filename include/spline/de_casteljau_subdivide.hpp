#pragma once

/// @file
/// @brief De Casteljau's algorithm with subdivision

#include <type_traits>
#include <utility>
#include "spline/concepts.hpp"
#include "spline/core/de_casteljau_subdivide.hpp"
#include "spline/traits.hpp"

/// @namespace spline
/// @brief The Spline library
namespace spline {

namespace detail {
struct de_casteljau_subdivide_fn final {
    /// @brief De Casteljau's algorithm with subdivision
    /// @tparam InputIt Input iterator.
    /// @tparam OutputIt Output iterator.
    /// @tparam Scalar Scalar number type (typically float/double/long double).
    /// @tparam MultiplicationOp binary operator accepting
    ///         InputIt::value_type and Scalar, returning an
    ///         InputIt::value_type.
    /// @tparam AdditionOp binary operator accepting two InputIt::value_type
    ///         and returning a InputIt::value_type.
    /// @param[in] first Iterator to first element of input range.
    /// @param[in] last Iterator to one past the last element of the range.
    /// @param[in] t Point at which to compute the polynomial value.
    /// @param[in] mul Vector-Scalar multiplication function object.
    /// @param[in] add Vector-Vector addition function object.
    /// @param[out] d_first Beginning of destination range.
    /// @return Output iterator to the element past the last element written.
    /// @pre @p d_first must point to a destination range where it is possible
    /// to write  2 * std::distance(first, last) - 1 elements.
    template <class InputIt, class OutputIt, class Scalar,
              class MultiplicationOp, class AdditionOp>
    constexpr auto operator()(InputIt first, InputIt last, OutputIt d_first,
                              Scalar t, MultiplicationOp mul,
                              AdditionOp add) const
        -> std::enable_if_t<
            concepts::is_vector_space_v<xtd::iter_value_t<InputIt>, Scalar,
                                        AdditionOp, MultiplicationOp>,
            OutputIt>
    {
        return core::de_casteljau_subdivide(std::move(first), std::move(last),
                                            std::move(d_first), std::move(t),
                                            std::move(mul), std::move(add));
    }
};
}  // namespace detail

/// @brief Global function object performing De Casteljau's algorithm with
///     subdivision
inline constexpr detail::de_casteljau_subdivide_fn de_casteljau_subdivide{};
}  // namespace spline
