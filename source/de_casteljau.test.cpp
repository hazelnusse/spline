#include <array>
#include <complex>
#include <spline/de_casteljau.hpp>
#include <ut.hpp>

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using spline::de_casteljau;

    using Point = std::complex<double>;

    test("EmptyPoly") = []() {
        auto c = std::array<Point, 0>{};

        expect(nullptr == de_casteljau(c.begin(), c.end(), 0.0));
    };

    test("Degree3Front") = []() {
        auto quad = std::array<Point, 4>{Point{0.0, 0.0}, Point{1.0, 1.0},
                                         Point{2.0, 0.0}, Point{3.0, -1.0}};

        constexpr auto expected = std::array<Point, 4>{
            Point{0.0, 0.0},  // b0^(0)
            Point{0.5, 0.5},  // b0^(1)
            Point{1.0, 0.5},  // b0^(2)
            Point{1.5, 0.25}  // b0^(3)
        };

        expect(expected.back() == *de_casteljau(quad.begin(), quad.end(), 0.5));
        expect(expected == quad);
    };

    test("Degree3Back") = []() {
        auto quad = std::array<Point, 4>{Point{0.0, 0.0}, Point{1.0, 1.0},
                                         Point{2.0, 0.0}, Point{3.0, -1.0}};

        constexpr auto expected = std::array<Point, 4>{
            Point{1.5, 0.25},  // b0^(3)
            Point{2.0, 0.0},   // b1^(2)
            Point{2.5, -0.5},  // b2^(1)
            Point{3.0, -1.0}   // b3^(0)
        };

        expect(expected.front() ==
               *de_casteljau(quad.rbegin(), quad.rend(), 1.0 - 0.5));
        expect(expected == quad);
    };
}
