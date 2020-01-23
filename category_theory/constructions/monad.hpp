#ifndef CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP
#define CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP

#include <type_traits>
#include "../utils/monad.hpp"
#include "../utils/utils.hpp"

namespace category_theory {
namespace monad {

template <class, class = std::void_t<>>
struct has_custom_join : std::false_type {};

template <class MMT>
struct has_custom_join<MMT,
                       std::void_t<decltype(category_theory_monad_custom_join(
                           std::declval<MMT>()))>> : std::true_type {};

template <class, class = std::void_t<>>
struct has_custom_bind : std::false_type {};

template <class MMT>
struct has_custom_bind<MMT,
                       std::void_t<decltype(category_theory_monad_custom_bind(
                           std::declval<MMT>()))>> : std::true_type {};

template <template <class> class, class>
constexpr static inline bool has_custom_join_v = false;

template <template <class> class, class>
constexpr static inline bool has_custom_bind_v = false;

struct join_t {
  template <class MMT,
            class T = extract_innermost_template_parameter_t<MMT>,
            class MT = collapse_outer_template_class_t<MMT>,
            class = std::enable_if_t<has_custom_join_v<MMT>>>
  [[nodiscard]] MT operator()(MMT&& m) {
    return category_theory_monad_custom_join(std::forward<MMT>(m));
  }

  template <class MMT,
            class T = extract_innermost_template_parameter_t<MMT>,
            class MT = collapse_outer_template_class_t<MMT>,
            class = std::enable_if_t<!has_custom_join_v<MMT>>,
            class = disambiguate<T>>
  [[nodiscard]] MT operator()(MMT&& m) {
    static_assert(fail<T>,
                  "A Monad must be an Applicative and have a "
                  "category_theory_monad_custom_join or "
                  "category_theory_monad_custom_bind definition")
  }
};

struct map_t {
  // TODO
};

}  // namespace monad
}  // namespace category_theory

#endif  // #ifndef CATEGORY_THEORY_CONSTRUCTIONS_MONAD_HPP