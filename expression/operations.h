#ifndef OPERATION_H
#define OPERATION_H

#include "../traits/traits.h"
#include "../operators/ops.h"
#include "../blob/shape.h"
#include "./expr.h"

namespace Mathub
{
template <typename Op, typename Arg>
struct UnaryOp : public Expr<UnaryOp<Op, Arg>>
{
    const Arg arg;

    static constexpr unsigned dim = Arg::dim;
    static constexpr Shape<dim> shape = Arg::shape;

    explicit UnaryOp(const Arg& arg) : arg(arg) {}

    inline auto eval(unsigned i) const
    {
        if constexpr (IsSame<Op, plus>::value)
            return arg.eval(i);
        else if constexpr (IsSame<Op, minus>::value)
            return -arg.eval(i);
    }
};

template <typename Op, typename Lhs, typename Rhs>
struct BinaryOp: public Expr<BinaryOp<Op, Lhs, Rhs>>
{
    const Lhs lhs;
    const Rhs rhs;

    static constexpr unsigned dim = CondValue<(Lhs::dim>Rhs::dim), Lhs::dim, Rhs::dim>::value;
    static constexpr Shape<dim> shape = Broadcasting<dim, Lhs::dim, Rhs::dim, Lhs, Rhs>::shape;

    explicit BinaryOp(const Lhs& lhs, const Rhs& rhs) : lhs(lhs), rhs(rhs) {}

    inline auto eval(unsigned i) const
    {
        if (Lhs::dim == Rhs::dim)
        {
            if (Lhs::shape[0] == Rhs::shape[0])
            {
                if constexpr (IsSame<Op, plus>::value)
                    return lhs.eval(i) + rhs.eval(i);
                else if constexpr (IsSame<Op, minus>::value)
                    return lhs.eval(i) - rhs.eval(i);
                else if constexpr (IsSame<Op, mul>::value)
                    return lhs.eval(i) * rhs.eval(i);
                else if constexpr (IsSame<Op, div>::value)
                    return lhs.eval(i) / rhs.eval(i);
            }
            else
            {
                if constexpr (IsSame<Op, plus>::value)
                    return (Lhs::shape[0] == 1) ? lhs.eval(0) + rhs.eval(i) : lhs.eval(i) + rhs.eval(0);
                else if constexpr (IsSame<Op, minus>::value)
                    return (Lhs::shape[0] == 1) ? lhs.eval(0) - rhs.eval(i) : lhs.eval(i) - rhs.eval(0);
                else if constexpr (IsSame<Op, mul>::value)
                    return (Lhs::shape[0] == 1) ? lhs.eval(0) * rhs.eval(i) : lhs.eval(i) * rhs.eval(0);
                else if constexpr (IsSame<Op, div>::value)
                    return (Lhs::shape[0] == 1) ? lhs.eval(0) / rhs.eval(i) : lhs.eval(i) / rhs.eval(0);
            }
        }
    }
};

template <typename Op, typename Arg>
inline UnaryOp<Op, Arg> F(const Arg& arg)
{
    return UnaryOp<Op, Arg>(arg.self());
}

template <typename Op, typename Lhs, typename Rhs>
inline BinaryOp<Op, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs)
{
    return BinaryOp<Op, Lhs, Rhs>(lhs.self(), rhs.self());
}
}

#endif
