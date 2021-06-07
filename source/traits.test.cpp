#include <array>
#include <complex>
#include <spline/traits.hpp>
#include <tuple>
#include <type_traits>
#include <ut.hpp>

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using namespace spline;

    auto add = [](auto v, auto u) -> decltype(v + u) { return v + u; };
    auto mul = [](auto a, auto v) -> decltype(a * v) { return a * v; };

    test("ValidVectorSpace") = []<class V>() {
        using Algebra =
            algebra::vector_space_algebra<double, std::decay_t<decltype(add)>,
                                          std::decay_t<decltype(mul)>>;

        expect(is_vector_space_v<V, Algebra>);
    } | std::tuple<std::complex<double>, double>{};

    test("VectorAndAlgebraMismatch") = []<class V>() {
        using Algebra =
            algebra::vector_space_algebra<double, std::decay_t<decltype(add)>,
                                          std::decay_t<decltype(mul)>>;

        expect(!is_vector_space_v<V, Algebra>);
    } | std::tuple<std::array<double, 2>, char>{};

    test("BadAdd") = [] {
        using V = std::array<double, 2>;
        using S = double;

        auto bad_add = [](auto, auto) -> S { return {}; };
        auto ok_mul = [](auto, auto) -> V { return {}; };

        using Algebra =
            algebra::vector_space_algebra<S, std::decay_t<decltype(bad_add)>,
                                          std::decay_t<decltype(ok_mul)>>;

        expect(!is_vector_space_v<V, Algebra>);
    };

    test("BadMul") = [] {
        using V = std::array<double, 2>;
        using S = double;

        auto ok_add = [](auto, auto) -> V { return {}; };
        auto bad_mul = [](auto, auto) -> S { return {}; };

        using Algebra =
            algebra::vector_space_algebra<S, std::decay_t<decltype(ok_add)>,
                                          std::decay_t<decltype(bad_mul)>>;

        expect(!is_vector_space_v<V, Algebra>);
    };
}
