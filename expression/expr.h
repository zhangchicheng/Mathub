#ifndef EXPR_H
#define EXPR_H

namespace Mathub
{
  template <typename SubType>
  struct Expr
  {
    inline const SubType& self() const
    {
      return *static_cast<const SubType*>(this);
    }
  };
}

#endif
