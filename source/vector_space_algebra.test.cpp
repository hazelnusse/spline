#include <complex>
#include <spline/vector_space_algebra.hpp>
#include <tuple>
#include <ut.hpp>

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using namespace spline::algebra;

    auto add = [](auto point1, auto point2) { return point1 + point2; };
    auto mul = [](auto real, auto point) { return real * point; };

    test("Constructible") = [add, mul]()
    {
        auto alg1 = make_vector_space<double>(add, mul);
        auto alg2 = vector_space_algebra{0.0, add, mul};

        expect(std::is_same_v<decltype(alg1), decltype(alg2)>);
        expect(std::is_same_v<
               vector_space_algebra<double, std::decay_t<decltype(add)>,
                                    std::decay_t<decltype(mul)>>,
               decltype(alg2)>);
    };

    test("VectorAddition") = [add, mul]()
    {
        auto alg = vector_space_algebra{0.0, add, mul};

        auto v = std::complex{1.0, 2.0};
        auto u = std::complex{3.0, 4.0};

        expect((v + u) == alg.add(v, u));
    };

    test("ScalarMultiplication") = [add, mul]()
    {
        auto alg = vector_space_algebra{0.0, add, mul};

        auto v = std::complex{1.0, 2.0};
        auto a = 3.0;

        expect((a * v) == alg.mul(a, v));
    };

    test("Copyable") = [add, mul]()
    {
        auto alg1 = vector_space_algebra{0.0, add, mul};
        auto alg2 = alg1;

        auto v = std::complex{1.0, 2.0};
        auto u = std::complex{3.0, 4.0};

        expect(alg1.add(v, u) == alg2.add(v, u));
    };

    test("Empty") = [add, mul]()
    {
        auto alg = vector_space_algebra{0.0, add, mul};

        expect(std::is_empty_v<decltype(alg)>);
    };

    test("WithMultipleVectorTypes") = []<class V>()
    {
        auto tuple_add = [](auto v, auto u) -> decltype(v) {
            return {std::get<0>(v) + std::get<0>(u),
                    std::get<1>(v) + std::get<1>(u)};
        };
        auto tuple_mul = [](auto a, auto v) -> decltype(v) {
            return {a * std::get<0>(v), a * std::get<1>(v)};
        };

        using Alg =
            vector_space_algebra<double, std::decay_t<decltype(tuple_add)>,
                                 std::decay_t<decltype(tuple_mul)>>;

        // using V = std::complex<double>;
        using S = typename Alg::scalar_type;

        expect(std::is_invocable_r_v<V, decltype(&Alg::template add<V, V>),
                                     Alg&, V, V>);
        expect(std::is_invocable_r_v<V, decltype(&Alg::template mul<S, V>),
                                     Alg&, S, V>);
    } | std::tuple<std::array<double, 2>, std::tuple<double, double>>{};
}
