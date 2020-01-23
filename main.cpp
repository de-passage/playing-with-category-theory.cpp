#include <iostream>
#include <iterator>
#include <optional>
#include <type_traits>
#include <vector>

#include "category_theory/constructions/applicative.hpp"
#include "category_theory/functor.hpp"

template <class T>
struct custom_type {
  T value;
};

template <class T>
constexpr custom_type<T> category_theory_applicative_custom_pure(
    category_theory::type_t<custom_type<T>>, T&& t) noexcept {
  return {std::forward<T>(t)};
}

static_assert(
    category_theory::applicative::has_custom_pure_v<custom_type, int>);

template <class T>
custom_type(T)->custom_type<T>;

template <class T, class F, class U = std::result_of_t<F(T)>>
constexpr custom_type<U> category_theory_functor_custom_map(F f,
                                                  const custom_type<T>& ct) {
  return {f(ct.value)};
}

static_assert(category_theory::functor::has_custom_map_v<custom_type<int>,
                                                         double (*)(int)>);

static_assert(!std::is_convertible_v<custom_type<int>, int>);
static_assert(!std::is_convertible_v<int, custom_type<int>>);

namespace category_theory {
template <class T, class U, class = std::void_t<>>
struct has_custom_to_function : std::false_type {};
template <class T, class U>
struct has_custom_to_function<
    T,
    U,
    std::void_t<decltype(category_theory_isomorphism_custom_to(
        type_t<U>{}, std::declval<T>()))>> : std::true_type {};

template <class T, class U>
constexpr static inline bool has_custom_to_function_v =
    has_custom_to_function<T, std::decay_t<U>>::value;

template <class T, class U>
constexpr static inline bool has_conversion_from_to =
    std::is_convertible_v<T, U> || has_custom_to_function_v<T, U>;

namespace isomorphism {
template <class T>
struct to_t {
  template <class U, class = std::enable_if_t<has_conversion_from_to<T, U>>>
  constexpr T operator()(U&& from) const noexcept {
    if constexpr (has_custom_to_function_v<T, U>) {
      return category_theory_isomorphism_custom_to(type_t<T>{},
                                                   std::forward<U>(from));
    }
    else {
      return from;
    }
  }
};
}  // namespace isomorphism

template <class T>
constexpr static inline auto to = isomorphism::to_t<T>{};

template <class T, class U>
constexpr static inline bool is_isomorphic =
    has_conversion_from_to<T, U>&& has_conversion_from_to<U, T>;

template <class T, class = std::enable_if_t<is_isomorphic<int, T>>>
constexpr bool operator<(const T& left, const T& right) noexcept {
  return to<int>(left) < to<int>(right);
}

}  // namespace category_theory

constexpr int category_theory_isomorphism_custom_to(
    category_theory::type_t<int>, custom_type<int> c) {
  return c.value;
}

constexpr custom_type<int> category_theory_isomorphism_custom_to(
    category_theory::type_t<custom_type<int>>, int c) {
  return {c};
}

constexpr auto times2 = [](auto&& f) constexpr {
  return category_theory::map(
      [](int i) constexpr { return i * 2; }, std::forward<decltype(f)>(f));
};

int main(int, char**) {
  using namespace category_theory;

  static_assert(category_theory::to<custom_type<int>>(0).value == 0);
  static_assert(category_theory::to<int>(custom_type{0}) == 0);

  auto i = pure<std::optional>(8);
  std::optional<int> j = std::nullopt;
  std::vector<int> v = {0, 1, 2, 3, 4};

  const auto o = times2(std::move(i));
  std::cout << "Hello, world!\n"
            << o.value() << std::boolalpha << " j * 2 has value? "
            << times2(j).has_value() << '\n';

  const auto v2 = times2(std::move(v));
  std::copy(
      std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, ", "));
  std::cout << '\n';
  std::copy(std::begin(v2),
            std::end(v2),
            std::ostream_iterator<int>(std::cout, ", "));

  constexpr custom_type ct = pure<custom_type>(42);
  constexpr auto ct2 = times2(ct);
  std::cout << ct2.value << '\n';

  static_assert(ct < ct2);
  static_assert(0 < 1);
}
