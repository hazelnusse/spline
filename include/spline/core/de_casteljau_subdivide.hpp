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
/// @tparam InputIt Input iterator. Must meet the requirements of
///         *LegacyInputIterator*. Defines (exposition-only) `Vector` as
///         `xtd::iter_value_t<InputIt>`.
/// @tparam OutputIt Output iterator. Must meet the requirements of
///         *LegacyBidirectionalIterator*. `xtd::iter_value_t<OutputIt>` must be
///         the same as `xtd::iter_value_t<InputIt>`.
/// @tparam Scalar Scalar type (typically `float`/`double`/`long double`). Must
///         be constructible with `Scalar{1}` and closed under subtraction.
/// @tparam Mul Binary operator accepting `Scalar` and `Vector`, returning a
///         `Vector`.
/// @tparam Add Binary operator accepting two `Vector` objects and returning a
///         `Vector`.
/// @param first Iterator to first element of input range.
/// @param last Iterator to one past the last element of the range.
/// @param d_first Beginning of destination range.
/// @param t Point at which to compute the polynomial value.
/// @param mul `Scalar`-`Vector` multiplication function object.
/// @param add `Vector`-`Vector` addition function object.
/// @return Output iterator to the element past the last element written.
/// @pre `first` and `last` must form a valid range.
/// @pre `t` is in the closed interval **[0, 1]**.
/// @pre `d_first` must point to a destination range where it is possible to
///      write `2 * distance(first, last) - 1` elements.
///
/// Evaluates a polynomial, in Bernstein form, at independent variable `t` using
/// De Casteljau's algorithm. **[first, last)** describe a range specifying
/// the Bernstein coefficients, where the polynomial degree **n =**
/// `distance(first, last) - 1`. Writes the coefficients from reduced
/// polynomials **[B0^(0), B0^(1), ..., B0^(n), ... Bn-1^(1), Bn^(0)]** in the
/// destination range `[d_first, next(2 * d_first - 1)]`.
///
/// To evaluate the polynomial in-place, see spline::de_casteljau.
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
