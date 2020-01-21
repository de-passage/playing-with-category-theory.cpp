#ifndef CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP
#define CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP

#include <type_traits>
#include "../utils/utils.hpp"
#include "../utils/monad.hpp"

namespace category_theory {
namespace monad {

template <template <class> class, class, class = std::void_t<>>
struct has_custom_join : std::false_type {};

template <template <class> class M, class T>
struct has_custom_join<M,
                       T,
                       std::void_t<decltype(category_theory_monad_custom_join(
                           std::declval<M<M<T>>>()))>> : std::true_type {};

template <template <class> class, class, class = std::void_t<>>
struct has_custom_bind : std::false_type {};

template <template <class> class M, class T>
struct has_custom_bind<M,
                       T,
                       std::void_t<decltype(category_theory_monad_custom_bind(
                           std::declval<M<T>>()))>> : std::true_type {};

template <template <class> class, class>
constexpr static inline bool has_custom_join_v = false;

template <template <class> class, class>
constexpr static inline bool has_custom_bind_v = false;

struct join_t {
  template <template <class> class M,
            class T,
            class MMT = M<M<T>>,
            class = std::enable_if_t<has_custom_join_v<M, T>>>
  [[nodiscard]] M<T> operator()(MMT&& m) {
    return category_theory_monad_custom_join(std::forward<MMT>(m));
  }

  template <class MMT,
            class T = extract_innermost_template_parameter_t<MMT>,
            class MMT = M<M<T>>,
            class = std::enable_if_t<!has_custom_join_v<M, T>>,
            class = disambiguate<T>>
  [[nodiscard]] M<T> operator()(MMT&& m) {
    static_assert(fail<T>,
                  "A Monad must be an Applicative and have a "
                  "category_theory_monad_custom_join or "
                  "category_theory_monad_custom_bind definition")
  }
};

struct map_t {
    template<template<class> 
};

}  // namespace monad
}  // namespace category_theory

#endif  // #ifndef CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP