#include <compare>
#include <spline/impl/algebraic_vector.hpp>
#include <ut.hpp>

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using namespace spline;

    struct point_t {
        double x;
        double y;

        auto operator<=>(const point_t&) const = default;
    };

    test("Construction") = [] {
        auto add = [](auto v, auto u) { return v + u; };
        auto mul = [](auto a, auto v) { return a * v; };
        auto alg = algebra::vector_space_algebra{0.0, add, mul};

        auto x = algebra::impl::algebraic_vector{0.0, alg};
        auto y = algebra::impl::algebraic_vector{point_t{1.0, 2.0}, alg};

        (void)x;
        (void)y;
        expect(true);
    };

    test("VectorAddition") = [] {
        using Scalar = double;

        auto alg =
            algebra::vector_space_algebra{Scalar{},
                                          [](point_t v, point_t u) -> point_t {
                                              return {v.x + u.x, v.y + u.y};
                                          },
                                          [](Scalar a, point_t v) -> point_t {
                                              return {a * v.x, a * v.y};
                                          }};

        auto v = algebra::impl::algebraic_vector{point_t{1.0, 2.0}, alg};
        auto u = algebra::impl::algebraic_vector{point_t{2.0, 1.0}, alg};

        expect((v + u).value == (u + v).value);
        expect(alg.add(v.value, u.value) == (v + u).value);
        expect(alg.add(u.value, v.value) == (u + v).value);
    };

    test("ScalarMultiplication") = [] {
        using Scalar = double;

        auto alg =
            algebra::vector_space_algebra{Scalar{},
                                          [](point_t v, point_t u) -> point_t {
                                              return {v.x + u.x, v.y + u.y};
                                          },
                                          [](Scalar a, point_t v) -> point_t {
                                              return {a * v.x, a * v.y};
                                          }};

        auto v = algebra::impl::algebraic_vector{point_t{1.0, 2.0}, alg};
        auto a = 3.0;

        expect((a * v).value == (v * a).value);
        expect(alg.mul(a, v.value) == (a * v).value);
    };
}
