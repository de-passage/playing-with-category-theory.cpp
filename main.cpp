#include <iostream>
#include <type_traits>
#include <optional>
#include <vector>
#include <iterator>
#include "category_theory/functor.hpp"

template <class T>
struct custom_type
{
    T value;
};

template <class T>
custom_type(T)->custom_type<T>;

template <class T, class F, class U = std::result_of_t<F(T)>>
custom_type<U> category_theory_functor_custom_map(F f, const custom_type<T> &ct)
{
    return {f(ct.value)};
}

int main(int, char **)
{
    using namespace category_theory;

    std::optional<int> i = 8;
    std::optional<int> j = std::nullopt;
    std::vector<int> v = {0, 1, 2, 3, 4};

    constexpr auto times2 = [] (auto&& f) {
        return map([](int i) { return i * 2; }, std::forward<decltype(f)>(f));
    };

    const auto o = times2(std::move(i));
    std::cout << "Hello, world!\n"
              << o.value() << std::boolalpha << " j * 2 has value? " << times2(j).has_value() << '\n';

    const auto v2 = times2(std::move(v));
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << '\n';
    std::copy(std::begin(v2), std::end(v2), std::ostream_iterator<int>(std::cout, ", "));

    custom_type ct {42};
    auto ct2 = times2(ct);
    std::cout << ct2.value << '\n';
}
