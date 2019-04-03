#ifndef OPS_H
#define OPS_H

#include "traits.h"

namespace Mathub
{

  template <typename T>
  struct mul
  {
    inline static T Map(T a, T b)
    {
      return a * b;
    }
  };

  template <typename T>
  struct plus
  {
    inline static T Map(T a, T b)
    {
      return a + b;
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
    static constexpr bool isPlus = isInstance<Op, plus>::value;
    static constexpr bool isMul = isInstance<Op, mul>::value;
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

    inline T Eval(unsigned i) const
    {
      return Op::Map(lhs.Eval(i), rhs.Eval(i));
    }

    inline T Eval(unsigned i, unsigned j) const
    {
      // lhs dim=1, rhs dim 2
      // lhs dim 2, rhs dim 1
      // lhs dim =2, rhs dim = 2
      if constexpr (isMul && dim==2)
      {
        T tmp=0;
        for (unsigned k=0; k<Lhs::shape[1]; ++k)
          tmp += Op::Map(lhs.Eval(i,k), rhs.Eval(k,j));
        return tmp;
      }
      else
      return Op::Map(lhs.Eval(i,j), rhs.Eval(i,j));
    }
  };

  template <typename Op, typename T, typename Lhs, typename Rhs>
  inline BinaryOp<Op, T, Lhs, Rhs>
  F(const Lhs& lhs, const Rhs& rhs)
  {
    return BinaryOp<Op, T, Lhs, Rhs>(lhs.self(), rhs.self());
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<mul<T>, T, Lhs, Rhs>
  operator*(const Lhs& lhs, const Rhs& rhs)
  {
    return F<mul<T>, T>(lhs, rhs);
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<plus<T>, T, Lhs, Rhs>
  operator+(const Lhs& lhs, const Rhs& rhs)
  {
    return F<plus<T>, T>(lhs, rhs);
  }
}

#endif
