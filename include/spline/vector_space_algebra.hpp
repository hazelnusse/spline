#pragma once

/// @file
/// @brief Representation of a vector space algebra

#include <type_traits>
#include <utility>
#include "stdx/traits.hpp"

/// @namespace algebra
/// @brief Types representing and creating vector space algebras
namespace spline::algebra {
/// @brief A generic vector space defining addition and scalar multiplication
/// @tparam Scalar Vector space scalar type
/// @tparam Addition Function object type defining vector addition
/// @tparam Multiplication Function object type defining scalar multiplication
/// @note The vector type is not specified, allowing a `vector_space_algebra` to
/// be used with multiple vector types.
template <class Scalar, class Addition, class Multiplication>
class vector_space_algebra : Addition, Multiplication {
  public:
    static_assert(std::is_empty_v<Addition>,
                  "Additional must be a stateless function object.");
    static_assert(std::is_empty_v<Multiplication>,
                  "Multiplication must be a stateless function object.");

    using scalar_type = Scalar;

    /// @brief Constructs a vector space from addition and multiplication
    /// operations
    /// @tparam A Function object type defining vector addition
    /// @tparam M Function object type defining scalar multiplication
    template <class A, class M,
              class = std::enable_if_t<
                  std::is_same_v<std::decay_t<A>, Addition> &&
                  std::is_same_v<std::decay_t<M>, Multiplication>>>
    constexpr vector_space_algebra(A&& add, M&& mul) :
        Addition{std::forward<A>(add)}, Multiplication{std::forward<M>(mul)}
    {}

    /// @brief Constructs a vector space from addition and multiplication
    /// operations, accepting the scalar type to allow
    // CTAD
    /// @tparam A Function object type defining vector addition
    /// @tparam M Function object type defining scalar multiplication
    template <class A, class M,
              class = std::enable_if_t<
                  std::is_same_v<std::decay_t<A>, Addition> &&
                  std::is_same_v<std::decay_t<M>, Multiplication>>>
    constexpr vector_space_algebra(Scalar, A&& add, M&& mul) :
        vector_space_algebra{std::forward<A>(add), std::forward<M>(mul)}
    {}

    /// @brief Performs vector addition
    template <class... Args>
    constexpr auto add(Args&&... args) const
        -> decltype(std::declval<Addition>()(std::forward<Args>(args)...))
    {
        return Addition::operator()(std::forward<Args>(args)...);
    }

    /// @brief Performs scalar multiplication
    template <class... Args>
    constexpr auto mul(Args&&... args) const
        -> decltype(std::declval<Multiplication>()(std::forward<Args>(args)...))
    {
        return Multiplication::operator()(std::forward<Args>(args)...);
    }
};

/// @brief Deduction guide
template <class Scalar, class Addition, class Multiplication>
vector_space_algebra(Scalar, Addition, Multiplication)
    -> vector_space_algebra<Scalar, Addition, Multiplication>;

/// @brief Creates a vector space, specifying Scalar but deducing Addition and
/// Multiplication
template <class Scalar, class Addition, class Multiplication>
constexpr auto make_vector_space(Addition&& add, Multiplication&& mul)
    -> vector_space_algebra<std::decay_t<Scalar>, std::decay_t<Addition>,
                            std::decay_t<Multiplication>>
{
    return {std::forward<Addition>(add), std::forward<Multiplication>(mul)};
}

/// @brief A template alias to determine if a type is a specialization of vector
/// space algebra
///@{
template <class T>
using is_vector_space_algebra =
    stdx::is_specialization_of<T, vector_space_algebra>;

template <class T>
inline constexpr bool is_vector_space_algebra_v =
    is_vector_space_algebra<T>::value;
///@}

}  // namespace spline::algebra
