#pragma once

/// @file
/// @brief De Casteljau's algorithm with subdivision, without constraint
/// checking

#include <iterator>
#include <numeric>

/// @namespace spline::core
/// @brief The Spline library algorithms
namespace spline::core {

/// @brief De Casteljau's algorithm with subdivision
/// @tparam InputIt Input iterator.
/// @tparam OutputIt Output iterator.
/// @tparam Scalar Scalar number type (typically float/double/long double).
/// @tparam Mul binary operator accepting
///         InputIt::value_type and Scalar, returning an
///         InputIt::value_type.
/// @tparam Add binary operator accepting two InputIt::value_type
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
template <class InputIt, class OutputIt, class Scalar, class Mul, class Add>
constexpr auto de_casteljau_subdivide(InputIt first, InputIt last,
                                      OutputIt d_first, Scalar t, Mul mul,
                                      Add add) -> OutputIt
{
    // initialize left half of destination range with b0^(0) .. bn^(0), then
    // [b0^(0), b1^(0), ..., bn^(0), X, ..., X, X]
    auto d_last = std::copy(first, last, d_first);
    if (d_last == d_first) {
        return d_last;
    }

    // point to mid and last element in destination range
    const auto n = std::distance(d_first, d_last) - 1;
    const auto d_mid = std::prev(d_last);
    std::advance(d_last, n - 1);

    // copy the bn^(0) to the last element in the dest
    // [b0^(0), b1^(0), ..., bn^(0), X, ..., X, bn^(0)]
    *d_last = *d_mid;

    // evaluate reduced order polynomials with recurrence relation
    for (; d_first != d_last; ++d_first) {
        // in first iteration:
        // [b0^(0), b0^(1), ..., bn-1^(1), X, ..., X, bn^(0)]
        std::adjacent_difference(
            d_first, std::next(d_mid), d_first, [t, mul, add](auto b, auto a) {
                return add(mul((Scalar{1} - t), a), mul(t, b));
            });

        // in first iteration:
        // [b0^(0), b0^(1), ..., bn-1^(1), X, ..., bn-1^(1), bn^(0)]
        *--d_last = *d_mid;
    }

    return std::next(d_mid, n + 1);
}

}  // namespace spline::core
