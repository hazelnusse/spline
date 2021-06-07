#include <array>
#include <spline/de_casteljau_subdivide.hpp>
#include <ut.hpp>

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using spline::de_casteljau_subdivide;

    auto mul = [](auto point, auto real) { return point * real; };

    auto add = [](auto point1, auto point2) { return point1 + point2; };

    test("EmptyOrReversed") = [mul, add]() {
        std::array<double, 0> c{};
        std::array<double, 0> result{};
        auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(), result.begin(),
                                          0.0, mul, add);
        expect(it1 == result.cbegin());
        expect(it1 == result.cend());
        auto it2 = de_casteljau_subdivide(c.cend(), c.cbegin(), result.begin(),
                                          0.0, mul, add);
        expect(it2 == result.cbegin());
        expect(it2 == result.cend());
    };

    test("Degree0") = [mul, add]() {
        {
            std::array<double, 1> c{42.0};
            std::array<double, 1> result{};
            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), 0.0, mul, add);
            expect(it1 == result.cend());
            expect(*--it1 == c[0]);
            expect(result[0] == c[0]);
        }
    };

    test("Degree1") = [mul, add]() {
        auto degree1poly = [](auto c, auto t) {
            return c[0] * (1 - t) + c[1] * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            std::array<double, 2> c{42.0, 43.0};
            std::array<double, 3> result{};
            result.fill(-35.0);
            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), t, mul, add);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == degree1poly(c, t));
            expect(result[2] == c[1]);
        }
    };

    test("Degree2") = [mul, add]() {
        auto degree2poly = [](auto omt, auto c, auto t) {
            return c[0] * omt * omt + 2.0 * c[1] * t * omt + c[2] * t * t;
        };
        for (int i = 0; i < 11; ++i) {
            double t = i * 0.1;
            auto const omt = 1.0 - t;
            std::array<double, 3> c{0.0, 2.0, 4.0};
            std::array<double, 5> result{};
            result.fill(-35.0);
            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), t, mul, add);
            expect(it1 == result.cend());
            expect(result[0] == c[0]);
            expect(result[1] == omt * c[0] + t * c[1]);
            expect(result[2] - degree2poly(omt, c, t) == 0.0_d);
            expect(result[3] == omt * c[1] + t * c[2]);
            expect(result[4] == c[2]);
        }
    };

    test("Degree3") = [mul, add]() {
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
            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), t, mul, add);
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

    test("Degree4") = [mul, add]() {
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
            auto it1 = de_casteljau_subdivide(c.cbegin(), c.cend(),
                                              result.begin(), t, mul, add);
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
