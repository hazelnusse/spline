#pragma once

/// @file
/// @brief De Casteljau's algorithm without subdivision, without constraint
/// checking

#include <iterator>
#include <numeric>

namespace spline::core {

template <class InputIt, class Scalar, class Mul, class Add>
constexpr auto de_casteljau(InputIt first, InputIt last, Scalar t, Mul mul,
                            Add add) -> InputIt
{
    if (first == last) {
        return {};
    }

    // evaluate reduced order polynomials with recurrence relation
    for (; first != last; ++first) {
        // in first iteration:
        // [b0^(0), b0^(1), ..., bn-1^(1)]
        std::adjacent_difference(
            first, last, first, [t, mul, add](auto b, auto a) {
                return add(mul((Scalar{1} - t), a), mul(t, b));
            });
    }

    return --first;
}

}  // namespace spline::core
