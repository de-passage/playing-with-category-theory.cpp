#ifndef CATEGORYTHEORY_UTILS_UTILS_HPP
#define CATEGORYTHEORY_UTILS_UTILS_HPP

#include <type_traits>

namespace category_theory {

template <class, template <typename...> class>
struct apply_template_parameters;

template <template <typename...> class T,
          template <class>
          class F,
          typename... Ps>
struct apply_template_parameters<T<Ps...>, F> {
  using value = F<Ps...>;
};

template <class T, template <typename...> class F>
using apply_template_parameters_t =
    typename apply_template_parameters<T, F>::type;

/// A template class that can never be instanciated
template <class T>
using never_t = std::enable_if_t<!std::is_same_v<T, T>>;

template <class T>
constexpr static bool fail = false;

namespace detail {

template <class T, class U, class = std::void_t<>>
struct is_direct_list_initializable : std::false_type {};

template <class T, template <class...> class U, class... Ts>
struct is_direct_list_initializable<
    T,
    U<Ts...>,
    std::void_t<decltype(T{std::declval<Ts>()...})>> : std::true_type {};

}  // namespace detail

template <class... Ts>
struct tuple {};

template <class T>
struct type_t {};

template <class T, class... Ts>
struct is_direct_list_initializable
    : detail::is_direct_list_initializable<T, tuple<Ts...>> {};

template <class T, class... Ts>
constexpr static inline bool is_direct_list_initializable_v =
    is_direct_list_initializable<T, Ts...>::value;

}  // namespace category_theory

#endif  // #ifndef CATEGORYTHEORY_UTILS_UTILS_HPP