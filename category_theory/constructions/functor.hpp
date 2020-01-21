#ifndef CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP
#define CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP

#include <type_traits>

#include "../utils/utils.hpp"

namespace category_theory {
namespace functor {

template <typename F, typename Fun, class = std::void_t<>>
struct has_custom_map : std::false_type {};

template <typename F, typename Fun>
struct has_custom_map<F,
                      Fun,
                      std::void_t<decltype(category_theory_functor_custom_map(
                          std::declval<Fun>(), std::declval<F>()))>>
    : std::true_type {};

template <typename F, typename Fun>
constexpr static inline bool has_custom_map_v = has_custom_map<F, Fun>::value;

struct map_t {
  template <class Fun,
            class F,
            class T = functor_value_t<F>,
            class U = std::result_of_t<Fun(T)>,
            class = std::enable_if_t<has_custom_map_v<F, Fun>>>
  [[nodiscard]] constexpr make_functor_t<F, U> operator()(Fun &&f, F &&g) const
      noexcept {
    return category_theory_functor_custom_map(std::forward<Fun>(f),
                                              std::forward<F>(g));
  }

  template <class Fun,
            class F,
            class = std::enable_if_t<!has_custom_map_v<F, Fun>>>
  [[nodiscard]] constexpr make_functor_t<F, int> operator()(Fun &&f,
                                                            F &&g) const
      noexcept {
    static_assert(fail<F>,
                  "Functor types require a category_theory_functor_custom_map "
                  "to be defined.");
  }
};

}  // namespace functor

constexpr static inline functor::map_t map;

}  // namespace category_theory

#endif  // #ifndef CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP