#include <array>
#include <complex>
#include <functional>
#include <spline/concepts.hpp>
#include <spline/de_casteljau_subdivide.hpp>
#include <ut.hpp>

namespace {

template <class T>
struct point {
    T x;
    T y;

    friend constexpr auto operator+(point v, point u) -> point
    {
        return {v.x + u.x, v.y + u.y};
    }

    friend constexpr auto operator*(T a, point v) -> point
    {
        return {a * v.x, a * v.y};
    }
};

template <class V, class S>
struct vs {
    using vector_type = V;
    using scalar_type = S;
};

}  // namespace

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using spline::de_casteljau_subdivide;

    test("VectorSpaceConstrained") = []() {
        using Scalar = double;
        using Vector = std::array<double, 2>;
        using Container = std::array<Vector, 3>;

        // Use SFINAE friendly lambdas
        const auto bad_mul = [](auto a, auto v) -> decltype(a * v) {
            return a * v;
        };
        const auto bad_add = [](auto v, auto u) -> decltype(v + u) {
            return v + u;
        };
        const auto ok_mul = [](auto a, auto v) -> decltype(v) {
            return {a * std::get<0>(v), a * std::get<1>(v)};
        };
        const auto ok_add = [](auto v, auto u) -> decltype(v) {
            return {std::get<0>(v) + std::get<0>(u),
                    std::get<1>(v) + std::get<1>(u)};
        };

        expect(not std::is_invocable_v<
               decltype(de_casteljau_subdivide), typename Container::iterator,
               typename Container::iterator, typename Container::iterator,
               Scalar, std::multiplies<>, std::plus<>>);

        expect(not std::is_invocable_v<
               decltype(de_casteljau_subdivide), typename Container::iterator,
               typename Container::iterator, typename Container::iterator,
               Scalar, decltype(bad_mul), decltype(bad_add)>);

        expect(not std::is_invocable_v<
               decltype(de_casteljau_subdivide), typename Container::iterator,
               typename Container::iterator, typename Container::iterator,
               Vector, decltype(ok_mul), decltype(ok_add)>);

        expect(std::is_invocable_v<
               decltype(de_casteljau_subdivide), typename Container::iterator,
               typename Container::iterator, typename Container::iterator,
               Scalar, decltype(ok_mul), decltype(ok_add)>);

        expect(std::is_invocable_v<
               decltype(de_casteljau_subdivide), typename Vector::iterator,
               typename Vector::iterator, typename Vector::iterator, double,
               std::multiplies<>, std::plus<>>);
    };

    test("UsesDefaultVectorSpaceOps") =
        []<class T>() {
            using V = typename T::vector_type;
            using S = typename T::scalar_type;
            std::array<V, 3> c{};
            std::array<V, 3> result{};

            // Fails to compile if the template parameter is ill-formed
            expect(spline::xtd::dependent_bool_v<
                   true, decltype(de_casteljau_subdivide(
                             c.begin(), c.end(), result.begin(), S{}))>);
        } |
        std::tuple<vs<point<double>, double>,
                   vs<std::complex<double>, double>>{};

    test("EmptyOrReversed") = []() {
        std::array<double, 0> c{};
        std::array<double, 0> result{};

        auto it1 =
            de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(), 0.0);
        expect(it1 == result.cbegin());
        expect(it1 == result.cend());

        auto it2 =
            de_casteljau_subdivide(c.cend(), c.cbegin(), result.begin(), 0.0);
        expect(it2 == result.cbegin());
        expect(it2 == result.cend());
    };

    test("Degree0") = []() {
        {
            std::array<double, 1> c{42.0};
            std::array<double, 1> result{};

            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), 0.0);
            expect(it1 == result.cend());
            expect(*--it1 == c[0]);
            expect(result[0] == c[0]);
        }
    };

    test("Degree1") = []() {
        auto degree1poly = [](auto c, auto t) {
            return c[0] * (1 - t) + c[1] * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            std::array<double, 2> c{42.0, 43.0};
            std::array<double, 3> result{};
            result.fill(-35.0);

            auto it1 =
                de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(), t);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == degree1poly(c, t));
            expect(result[2] == c[1]);
        }
    };

    test("Degree2") = []() {
        auto degree2poly = [](auto omt, auto c, auto t) {
            return c[0] * omt * omt + 2.0 * c[1] * t * omt + c[2] * t * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            auto const omt = 1.0 - t;
            std::array<double, 3> c{0.0, 2.0, 4.0};
            std::array<double, 5> result{};
            result.fill(-35.0);

            auto it1 =
                de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(), t);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == omt * c[0] + t * c[1]);
            expect(result[2] - degree2poly(omt, c, t) == 0.0_d);
            expect(result[3] == omt * c[1] + t * c[2]);
            expect(result[4] == c[2]);
        }
    };

    test("Degree3") = []() {
        auto degree3poly = [](auto omt, auto c, auto t) {
            return c[0] * omt * omt * omt + 3.0 * c[1] * t * omt * omt +
                   3.0 * c[2] * omt * t * t + c[3] * t * t * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            auto const omt = 1.0 - t;
            std::array<double, 4> c{0.0, 2.0, 4.0, 6.0};
            std::array<double, 7> result{};
            result.fill(-35.0);

            auto it1 =
                de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(), t);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == omt * c[0] + t * c[1]);
            expect(result[2] == omt * result[1] + t * (omt * c[1] + t * c[2]));
            expect(result[3] - degree3poly(omt, c, t) == 0.0_d);
            expect(result[4] == omt * (omt * c[1] + t * c[2]) + t * result[5]);
            expect(result[5] == omt * c[2] + t * c[3]);
            expect(result[6] == c[3]);
        }
    };

    test("Degree4") = []() {
        auto degree4poly = [](auto omt, auto c, auto t) {
            return c[0] * omt * omt * omt * omt +
                   4.0 * c[1] * t * omt * omt * omt +
                   6.0 * c[2] * omt * omt * t * t +
                   4.0 * c[3] * omt * t * t * t + c[4] * t * t * t * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            auto const omt = 1.0 - t;
            std::array<double, 5> c{0.0, 2.0, 4.0, 6.0, 8.0};
            std::array<double, 9> result{};
            result.fill(-35.0);

            auto it1 =
                de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(), t);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == omt * c[0] + t * c[1]);
            expect(result[2] == omt * result[1] + t * (omt * c[1] + t * c[2]));
            expect(result[3] ==
                   omt * result[2] + t * (omt * (omt * c[1] + t * c[2]) +
                                          t * (omt * c[2] + t * c[3])));
            expect(result[4] - degree4poly(omt, c, t) == 0.0_d);
            expect(result[5] == omt * (omt * (omt * c[1] + t * c[2]) +
                                       t * (omt * c[2] + t * c[3])) +
                                    t * result[6]);
            expect(result[6] == omt * (omt * c[2] + t * c[3]) + t * result[7]);
            expect(result[7] == omt * c[3] + t * c[4]);
            expect(result[8] == c[4]);
        }
    };
}
