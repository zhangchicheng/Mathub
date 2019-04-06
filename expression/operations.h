#ifndef OPERATION_H
#define OPERATION_H

#include "../traits/traits.h"
#include "../operators/ops.h"
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

  template <typename Op, typename T, typename Lhs, typename Rhs>
  struct BinaryOp: public Expr<BinaryOp<Op, T, Lhs, Rhs>>
  {
    using DataType = T;
    const Lhs& lhs;
    const Rhs& rhs;

    static constexpr unsigned LhsDim = Lhs::dim;
    static constexpr unsigned RhsDim = Rhs::dim;
    static constexpr bool isMul = isInstance<Op, mul>::value;

    //calculate output dim. this is just a workaround. it should be inferenced by op, lhs, and rhs.
    static constexpr unsigned dim = CondValue<(Lhs::dim>Rhs::dim), Lhs::dim, Rhs::dim>::value;

    explicit BinaryOp(const Lhs& lhs, const Rhs& rhs) : lhs(lhs), rhs(rhs) {}

    constexpr Shape<dim> getShape()
    {
      if constexpr (isMul && dim==2)
        return Shape<dim>{Lhs::shape[0], Rhs::shape[1]};
      else
        return Lhs::shape;
    }

    static constexpr Shape<dim> shape = getShape();

    inline T eval(unsigned i) const
    {
      return Op::map(lhs.eval(i), rhs.eval(i));
    }

    inline T eval(unsigned i, unsigned j) const
    {
      if constexpr (isMul && dim==2)
      {
        T tmp=0;
        for (unsigned k=0; k<Lhs::shape[1]; ++k)
          tmp += Op::map(lhs.eval(i,k), rhs.eval(k,j));
        return tmp;
      }
      else
        return Op::map(lhs.eval(i,j), rhs.eval(i,j));
    }
  };

  template <typename Op, typename T, typename Arg>
  inline UnaryOp<Op, T, Arg> F(const Arg& arg)
  {
    return UnaryOp<Op, T, Arg>(arg.self());
  }

  template <typename Op, typename T, typename Lhs, typename Rhs>
  inline BinaryOp<Op, T, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs)
  {
    return BinaryOp<Op, T, Lhs, Rhs>(lhs.self(), rhs.self());
  }
}

#endif
