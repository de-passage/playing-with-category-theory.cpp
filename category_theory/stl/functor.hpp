#ifndef CATEGORY_THEORY_STL_FUNCTOR_HPP
#define CATEGORY_THEORY_STL_FUNCTOR_HPP

#include <vector>
#include <iterator>
#include <optional>
#include <algorithm>
#include <type_traits>

#include "../utils/functor.hpp"

namespace category_theory::functor {

template <class F, class O, class T = category_theory::functor_value_t<O>, class U = std::result_of_t<F(T)>, class = std::enable_if_t<std::is_same_v<std::optional<T>, std::decay_t<O>>>>
std::optional<U> category_theory_functor_custom_map(F &&f, O &&v)
{
    if (v.has_value())
    {
        return std::optional<U>(std::forward<F>(f)(*std::forward<std::optional<T>>(v)));
    }
    else
    {
        return std::optional<U>();
    }
}

template <class F, class T, class U = std::result_of_t<F(T)>, class = std::enable_if_t<std::is_same_v<T, U>>>
std::optional<T> category_theory_functor_custom_map(F &&f, std::optional<T> &&v)
{
    if (v.has_value())
    {
        v = f(std::move(v).value());
        return std::move(v);
    }
    else
    {
        return std::optional<U>();
    }
}

template <class F, class V,
          class T = category_theory::functor_value_t<V>,
          class U = std::result_of_t<F(T)>,
          class R = category_theory::make_functor_t<V, U>,
          class = std::enable_if_t<is_of_functor_type_v<V, std::vector>>>
R category_theory_functor_custom_map(F &&f, const V &v)
{
    R us;
    us.reserve(v.size());
    std::transform(
        std::begin(v),
        std::end(v),
        std::back_inserter(us),
        std::forward<F>(f));
    return us;
}

template <class F, class V,
          class T = category_theory::functor_value_t<V>,
          class U = std::result_of_t<F(T)>,
          class R = category_theory::make_functor_t<V, U>,
          class = std::enable_if_t<!std::is_same_v<T, U>>>
R category_theory_functor_custom_map(F &&f, V &&v)
{
    V us;
    us.reserve(v.size());
    std::transform(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v)),
        std::back_inserter(us),
        std::forward<F>(f));
    return us;
}

template <class F, class U, class... Ps, class = std::enable_if_t<std::is_same_v<std::result_of_t<F(U)>, U>>>
std::vector<U, Ps...> category_theory_functor_custom_map(F &&f, std::vector<U, Ps...> &&v)
{
    for (auto &e : v)
        e = f(e);
    return std::move(v);
}

} // namespace category_theory::functor

#endif // #ifndef CATEGORY_THEORY_STL_FUNCTOR_HPP