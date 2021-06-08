#pragma once

/// @file
/// @brief A vector packaged with its vector space algebra

#include "../vector_space_algebra.hpp"

namespace spline::algebra::impl {

/// @brief A vector type defining overloaded operators for vector addition and
/// scalar multiplication
/// @tparam Underlying Vector type
/// @tparam Algebra Specialization of `vector_space_algebra` defining addition
/// and multiplication
template <class Underlying, class Algebra>
struct algebraic_vector : Algebra {
    static_assert(
        is_vector_space_algebra_v<Algebra>,
        "Algebra must be a specialization of `algebra::vector_space_algebra`.");

    using algebra_type = Algebra;
    using underlying_type = Underlying;
    using scalar_type = typename Algebra::scalar_type;

    constexpr algebraic_vector(underlying_type v, algebra_type a) :
        Algebra{std::move(a)}, value{std::move(v)}
    {}

    /// @brief Adds two vectors
    constexpr auto operator+(algebraic_vector u) const -> algebraic_vector
    {
        auto alg = static_cast<const Algebra&>(*this);
        return {alg.add(value, u.value), alg};
    }

    /// @brief Right multiplies a vector by a scalar
    constexpr auto operator*(scalar_type a) const -> algebraic_vector
    {
        auto alg = static_cast<const Algebra&>(*this);
        return {alg.mul(a, value), alg};
    }

    /// @brief Left multiplies a vector by a scalar
    friend constexpr auto operator*(scalar_type a, algebraic_vector v)
    {
        return v * a;
    }

    underlying_type value;
};

}  // namespace spline::algebra::impl
