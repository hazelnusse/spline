#pragma once

/// @file
/// @brief De Casteljau's algorithm without subdivision, without constraint
/// checking

#include <iterator>
#include <numeric>

namespace spline::core {

/// @brief In-place evaluation of De Casteljau's algorithm
/// @tparam InputIt Input iterator. Must meet the requirements of
///         *LegacyBidirectionalIterator*. Defines (exposition-only) `Vector` as
///         `xtd::iter_value_t<InputIt>`.
/// @tparam Scalar Scalar type (typically `float`/`double`/`long double`). Must
///         be constructible with `Scalar{1}` and closed under subtraction.
/// @tparam Mul Binary operator accepting `Scalar` and `Vector`, returning a
///         `Vector`.
/// @tparam Add Binary operator accepting two `Vector` objects and returning a
/// `Vector`.
/// @param first Iterator to first element of input range.
/// @param last Iterator to one past the last element of the range.
/// @param t Point at which to compute the polynomial value.
/// @param mul `Scalar`-`Vector` multiplication function object.
/// @param add `Vector`-`Vector` addition function object.
/// @return Iterator to the evaluated polynomial at point `t`.
/// @pre `first` and `last` must form a valid range.
/// @pre `t` is in the closed interval **[0, 1]**.
///
/// Evaluates a polynomial, in Bernstein form, at independent variable `t` using
/// De Casteljau's algorithm. **[first, last)** describe a range specifying
/// the Bernstein coefficients, where the polynomial degree **n =**
/// `distance(first, last) - 1`. Writes the coefficients from reduced
/// polynomials **[B0^(0), B0^(1), ... B0^(n)]** in the provided input range and
/// returns an iterator to **B0^(n)**.
///
/// The last coefficient of the reduced polynomials can be obtained by
/// supplying reverse iterators and `1 - t`:
///
/// ```
/// de_casteljau(c.rbegin(), c.rend(), 1 - t)
/// ```
///
/// To obtain the first and last coefficients of reduced polynomials, see
/// de_casteljau_subdivide().
///
/// @see spline::de_casteljau
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
