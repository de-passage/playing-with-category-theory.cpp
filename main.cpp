#include "category_theory/constructions/applicative.hpp"
#include "category_theory/functor.hpp"

#include <igloo/igloo.h>
#include <iostream>
#include <iterator>
#include <optional>
#include <type_traits>
#include <vector>

template <class T> struct custom_type { T value; };

template <class T>
constexpr custom_type<T>
category_theory_applicative_custom_pure(category_theory::type_t<custom_type<T>>, T &&t) noexcept {
  return {std::forward<T>(t)};
}

static_assert(
    category_theory::applicative::has_custom_pure_v<custom_type, int>);

template <class T> custom_type(T)->custom_type<T>;

template <class T, class F, class U = std::result_of_t<F(T)>>
custom_type<U> category_theory_functor_custom_map(F f,
                                                  const custom_type<T> &ct) {
  return {f(ct.value)};
}

int main(int, char **) {
  using namespace category_theory;

  auto i = pure<std::optional>(8);
  std::optional<int> j = std::nullopt;
  std::vector<int> v = {0, 1, 2, 3, 4};

  constexpr auto times2 = [](auto &&f) {
    return map([](int i) { return i * 2; }, std::forward<decltype(f)>(f));
  };

  const auto o = times2(std::move(i));
  std::cout << "Hello, world!\n"
            << o.value() << std::boolalpha << " j * 2 has value? "
            << times2(j).has_value() << '\n';

  const auto v2 = times2(std::move(v));
  std::copy(std::begin(v), std::end(v),
            std::ostream_iterator<int>(std::cout, ", "));
  std::cout << '\n';
  std::copy(std::begin(v2), std::end(v2),
            std::ostream_iterator<int>(std::cout, ", "));

  custom_type ct = pure<custom_type>(42);
  auto ct2 = times2(ct);
  std::cout << ct2.value << '\n';
}
