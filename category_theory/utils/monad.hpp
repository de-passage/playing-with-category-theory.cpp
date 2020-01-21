#ifndef CATEGORY_THEORY_UTILS_MONAD_HPP
#define CATEGORY_THEORY_UTILS_MONAD_HPP

namespace category_theory::monad {

template <class T>
struct extract_innermost_template_parameter {
  using type = T;
};

template <template <class> class M, class T>
struct extract_innermost_template_parameter<M<T>>
    : extract_innermost_template_parameter<T> {
}

template <class T>
using extract_innermost_template_parameter_t = typename extract_innermost_template_parameter<T>::type;

}  // namespace category_theory::monad

#endif  // #ifndef CATEGORY_THEORY_UTILS_MONAD_HPP