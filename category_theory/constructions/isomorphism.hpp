#ifndef CATEGORY_THEORY_CONSTRUCTIONS_ISOMORPHISM_HPP
#define CATEGORY_THEORY_CONSTRUCTIONS_ISOMORPHISM_HPP

#include <type_traits>
#include "../utils/utils.hpp"

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

  template <class U, class = std::enable_if_t<!has_conversion_from_to<T, U>>>
  constexpr T operator()(...) const noexcept {
    static_assert(
        fail<U>,
        "The to<A>(B) function requires either an implicit conversion from B "
        "to A or a category_theory_isomorphism_custom_to function.");
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

#endif  // CATEGORY_THEORY_CONSTRUCTIONS_ISOMORPHISM_HPP