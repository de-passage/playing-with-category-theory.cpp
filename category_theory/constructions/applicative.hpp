#ifndef CATEGORY_THEORY_CONSTRUCTIONS_APPLICATIVE_HPP
#define CATEGORY_THEORY_CONSTRUCTIONS_APPLICATIVE_HPP

#include <type_traits>

#include "../utils/utils.hpp"

namespace category_theory {
template <template <class> class A, class T, class = never_t<T>>
constexpr A<T> category_theory_applicative_custom_pure(type_t<A<T>>,
                                                       T&&) noexcept {
  throw;
}

namespace applicative {

template <template <class> class A, class T, class = std::void_t<>>
struct has_custom_pure : std::false_type {};

template <template <class> class A, class T>
struct has_custom_pure<
    A,
    T,
    std::void_t<decltype(category_theory_applicative_custom_pure(
        type_t<A<T>>{}, std::declval<T>()))>> : std::true_type {};

template <template <class> class A, class T>
constexpr static inline bool has_custom_pure_v = has_custom_pure<A, T>::value;

template <template <class> class A, class T>
constexpr static inline bool fits_auto_implementation_requirements =
    (std::is_pod_v<A<T>> && is_direct_list_initializable_v<A<T>, T>)
    || has_custom_pure_v<A, T> || std::is_constructible_v<A<T>, T>;

template <template <typename> class A>
struct pure_t {
  template <
      class T,
      class = std::enable_if_t<fits_auto_implementation_requirements<A, T>>>
  [[nodiscard]] constexpr A<T> operator()(T&& value) const noexcept {
    if constexpr (has_custom_pure_v<A, T>) {
      return category_theory_applicative_custom_pure(type_t<A<T>>{},
                                                     std::forward<T>(value));
    }
    else {
      return A<T>{std::forward<T>(value)};
    }
  }

  template <
      class T,
      class = std::enable_if_t<!fits_auto_implementation_requirements<A, T>>,
      class = T>
  [[nodiscard]] constexpr A<T> operator()(T&& value) const noexcept {
    static_assert(
        fail<T>,
        "Applicative types need to either be either constructible from a "
        "type matching their template parameter, or have a "
        "category_theory_applicative_custom_pure function defined.");
  };
};

}  // namespace applicative

template <template <class> class T>
constexpr static inline auto pure = applicative::pure_t<T>{};

}  // namespace category_theory

#endif  // #ifndef CATEGORY_THEORY_CONSTRUCTIONS_APPLICATIVE_HPP