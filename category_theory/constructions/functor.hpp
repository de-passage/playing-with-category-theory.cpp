#ifndef CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP
#define CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP

#include <type_traits>

namespace category_theory
{
namespace functor
{

struct map_t
{
    template <class Fun, class F, class T = functor_value_t<F>, class U = std::result_of_t<Fun(T)>>
    [[nodiscard]] constexpr make_functor_t<F, U> operator()(Fun &&f, F &&g) const
    {
        return category_theory_functor_custom_map(std::forward<Fun>(f), std::forward<F>(g));
    }
};

} // namespace functor

constexpr static inline functor::map_t map;

} // namespace category_theory

#endif // #ifndef CATEGORYTHEORY_CATEGORIES_FUNCTOR_HPP