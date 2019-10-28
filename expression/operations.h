#ifndef OPERATION_H
#define OPERATION_H

#include "../traits/traits.h"
#include "../operators/ops.h"
#include "../blob/shape.h"
#include "./expr.h"

namespace Mathub
{
template <typename Op, typename T, typename Arg>
struct UnaryOp : public Expr<UnaryOp<Op, T, Arg>>
{
    using DataType = T;
    const Arg& arg;

    static constexpr unsigned ArgDim = Arg::dim;

    //not a good way to determine output dimension.
    static constexpr unsigned dim = ArgDim;

    explicit UnaryOp(const Arg& arg) : arg(arg) {}

    inline T eval(unsigned i) const
    {
        return Op::map(arg.eval(i));
    }

    inline T eval(unsigned i, unsigned j) const
    {
        return Op::map(arg.eval(i,j));
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
                return lhs.eval(i) + rhs.eval(i);
            }
            else
            {
                return (Lhs::shape[0] == 1) ? lhs.eval(0) + rhs.eval(i) : lhs.eval(i) + rhs.eval(0);
            }
        }
    }
};

template <typename Op, typename T, typename Arg>
inline UnaryOp<Op, T, Arg> F(const Arg& arg)
{
    return UnaryOp<Op, T, Arg>(arg.self());
}

template <typename Op, typename Lhs, typename Rhs>
inline BinaryOp<Op, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs)
{
    return BinaryOp<Op,Lhs, Rhs>(lhs.self(), rhs.self());
}
}

#endif
