#ifndef CATEGORYTHEORY_UTILS_UTILS_HPP
#define CATEGORYTHEORY_UTILS_UTILS_HPP

namespace category_theory {

template<class, template<typename...> class>
struct apply_template_parameters;

template<template<typename...> class T, template<class> class F, typename...Ps>
struct apply_template_parameters<T<Ps...>, F> {
    using value = F<Ps...>;
};

template<class T, template<typename...> class F>
using apply_template_parameters_t = typename apply_template_parameters<T, F>::type;

}

#endif // #ifndef CATEGORYTHEORY_UTILS_UTILS_HPP  