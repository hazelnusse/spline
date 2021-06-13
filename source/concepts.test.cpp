#include <array>
#include <complex>
#include <functional>
#include <spline/concepts.hpp>
#include <tuple>
#include <ut.hpp>
#include <utility>

namespace {

template <class V, class S = typename V::value_type, class Add = std::plus<>,
          class Mul = spline::multiplies<S, V>>
struct vs {
    using vector_type = V;
    using scalar_type = S;
    using add_type = Add;
    using mul_type = Mul;
};
}  // namespace

auto main() -> int
{
    using namespace boost::ut::literals;
    using namespace boost::ut;
    using boost::ut::eq;
    using boost::ut::expect;
    using boost::ut::test;
    using namespace spline::concepts;

    auto add = [](auto v, auto u) -> decltype(decltype(v){
                                      std::get<0>(v) + std::get<0>(u),
                                      std::get<1>(v) + std::get<1>(u)}) {
        return {std::get<0>(v) + std::get<0>(u),
                std::get<1>(v) + std::get<1>(u)};
    };
    auto mul = [](auto a, auto v) -> decltype(decltype(v){a * std::get<0>(v),
                                                          a * std::get<1>(v)}) {
        return {a * std::get<0>(v), a * std::get<1>(v)};
    };

    (void)add;
    (void)mul;

    test("is_field") = []<class T>() { expect(is_field_v<T>); } |
                       std::tuple<int, float, double, std::complex<float>,
                                  std::complex<double>>{};

    test("is_not_field") =
        []<class T>() { expect(not is_field_v<T>); } |
        std::tuple<std::array<double, 2>, std::tuple<double, double>,
                   std::pair<double, double>>{};

    test("is_vector_space") =
        []<class T>() {
            expect(
                is_vector_space_v<typename T::vector_type,
                                  typename T::scalar_type, typename T::add_type,
                                  typename T::mul_type>);
        } |
        std::tuple<
            vs<int, int>, vs<double, double>, vs<std::complex<float>>,
            vs<std::complex<double>>,
            vs<std::array<double, 2>, double, decltype(add), decltype(mul)>,
            vs<std::tuple<double, double>, double, decltype(add),
               decltype(mul)>,
            vs<std::pair<double, double>, double, decltype(add),
               decltype(mul)>>{};

    test("is_not_vector_space") =
        []<class T>() {
            expect(not is_vector_space_v<
                   typename T::vector_type, typename T::scalar_type,
                   typename T::add_type, typename T::mul_type>);
        } |
        std::tuple<vs<std::array<double, 2>>,
                   vs<std::tuple<double, double>, double>,
                   vs<std::pair<double, double>, double>>{};
}
