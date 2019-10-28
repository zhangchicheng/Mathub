#ifndef SHAPE_H
#define SHAPE_H

#include "../traits/traits.h"
namespace Mathub
{
template <unsigned N>
struct Shape
{
    unsigned shape[N] = {};
    constexpr unsigned &operator[](unsigned i) { return shape[i]; }
    constexpr const unsigned &operator[](unsigned i) const { return shape[i]; }
};

template <>
struct Shape<0>
{
    unsigned shape = {};
};

template <unsigned N, unsigned N1, unsigned N2, typename Lhs, typename Rhs, unsigned ...D>
struct Broadcasting : Broadcasting<N - 1, N1 - 1, N2 - 1, Lhs, Rhs, CondValue<(Lhs::shape[N1 - 1] == Rhs::shape[N2 - 1]),
                                            2 * Lhs::shape[N1 - 1] * Rhs::shape[N2 - 1] / (Lhs::shape[N1 - 1] + Rhs::shape[N2 - 1]),
                                            CondValue<(Lhs::shape[N1 - 1] == 1 or Rhs::shape[N2 - 1] == 1),
                                            Lhs::shape[N1 - 1] * Rhs::shape[N2 - 1], 0>::value>::value, D...>
{
};

template <unsigned N, unsigned N2, typename Lhs, typename Rhs, unsigned ...D>
struct Broadcasting <N, 0, N2, Lhs, Rhs, D...> : Broadcasting<N - 1, 0, N2 - 1, Lhs, Rhs, Rhs::shape[N2 - 1], D...>
{
};

template <unsigned N, unsigned N1, typename Lhs, typename Rhs, unsigned ...D>
struct Broadcasting <N, N1, 0, Lhs, Rhs, D...> : Broadcasting<N - 1, N1 - 1, 0, Lhs, Rhs, Lhs::shape[N1 - 1], D...>
{
};

template <typename Lhs, typename Rhs, unsigned ...D>
struct Broadcasting <0, 0, 0, Lhs, Rhs, D...>
{
    static constexpr Shape<sizeof...(D)> shape = {D...};
};

/*
template<unsigned D, unsigned D1, unsigned D2, typename Lhs, typename Rhs>
struct Broadcasting
{
    static Shape<D> shape;
    static const unsigned dummy;
};

template<unsigned D, unsigned D1, unsigned D2, typename Lhs, typename Rhs>
const unsigned Broadcasting<D, D1, D2, Lhs, Rhs>::dummy = Broadcasting<D, D1, D2, Lhs, Rhs>::shape[D - 1]
                                                                     = Lhs::shape[D1 - 1] == Rhs::shape[D2 - 1] ?
                                                                       Lhs::shape[D1 - 1] :
                                                                       Lhs::shape[D1 - 1] * Rhs::shape[D2 - 1] + 0
                                                                           * Broadcasting<D - 1,
                                                                                                 D1 - 1,
                                                                                                 D2 - 1,
                                                                                                 Lhs,
                                                                                                 Rhs>::dummy;
*/
}

#endif
