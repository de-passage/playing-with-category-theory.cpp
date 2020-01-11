#ifndef CATEGORYTHEORY_UTILS_FUNCTOR_HPP
#define CATEGORYTHEORY_UTILS_FUNCTOR_HPP

#include <type_traits>

#include "./utils.hpp"

namespace category_theory
{

template <class>
struct functor_traits;

template <template <typename, typename...> class F, typename T, typename... Ts>
struct functor_traits<F<T, Ts...>>
{
    using value = T;

    template <typename U>
    using make_functor = F<U, Ts...>;
};

template <class F, class T>
using make_functor_t = typename functor_traits<std::remove_reference_t<F>>::template make_functor<T>;

template <class F>
using functor_value_t = typename functor_traits<std::remove_reference_t<F>>::value;

template<typename T, template<typename...> class F>
struct is_of_functor_type : std::is_same<apply_template_parameters<T, F>, T> {
};

template<typename T, template<typename...> class F>
constexpr static inline bool is_of_functor_type_v = is_of_functor_type<T, F>::value;

}

#endif // #ifndef CATEGORYTHEORY_UTILS_FUNCTOR_HPP