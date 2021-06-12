#include <algorithm>
#include <charconv>
#include <cstring>
#include <functional>
#include <limits>
#include <numeric>
#include <random>
#include <spline/de_casteljau_subdivide.hpp>
#include <vector>

namespace {
template <class Real>
constexpr auto nan = std::numeric_limits<Real>::quiet_NaN();

template <typename Real>
struct Point2d {
    Real data[2];

    constexpr Point2d<Real> operator+(Point2d<Real> const& other)
    {
        return {other.data[0] + data[0], other.data[1] + data[1]};
    }

    constexpr Point2d<Real> operator*(Real const a)
    {
        return {data[0] * a, data[1] * a};
    }

    constexpr Real operator[](std::size_t i) const { return data[i]; }
};

void example1(std::size_t N)
{
    using Point = Point2d<double>;

    std::vector<Point> input;
    input.reserve(N);
    std::vector<Point> output;
    output.reserve(2 * N - 1);
    constexpr Point nan_point{nan<double>, nan<double>};
    std::fill(output.begin(), output.end(), nan_point);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    double const dt = 2.0 / double(N - 1);
    for (std::size_t i = 0; i < N; ++i) {
        // Point const random_point{dis(gen), dis(gen)};
        Point const random_point{-1.0 + dt * double(i), dis(gen)};
        input.push_back(random_point);
    }
    spline::de_casteljau_subdivide(
        input.cbegin(), input.cend(), output.begin(), 0.5,
        [](auto a, auto t) -> Point {
            return Point{a[0] * t, a[1] * t};
        },
        [](auto a, auto b) -> Point {
            return Point{a[0] + b[0], a[1] + b[1]};
        });
}

}  // namespace

int main(int argc, char** argv)
{
    if (argc != 2) {
        return 1;
    }

    std::size_t N = 0;
    if (auto [p, ec] =
            std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), N);
        ec == std::errc()) {
        example1(N);
    } else {
        return 1;
    }

    return 0;
}
