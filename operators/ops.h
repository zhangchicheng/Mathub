#ifndef OPS_H
#define OPS_H

namespace Mathub
{
  template <typename Op, typename T, typename Arg> struct UnaryOp;
  template <typename Op, typename T, typename Lhs, typename Rhs> struct BinaryOp;

  template <typename Op, typename T, typename Arg>
  inline UnaryOp<Op, T, Arg> F(const Arg& arg);
  template <typename Op, typename T, typename Lhs, typename Rhs>
  inline BinaryOp<Op, T, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs);

  template <typename T>
  struct plus
  {
    inline static T map(T a, T b)
    {
      return a + b;
    }

    inline static T map(T a)
    {
      return a;
    }
  };

  template <typename T>
  struct minus
  {
    inline static T map(T a, T b)
    {
      return a - b;
    }

    inline static T map(T a)
    {
      return -a;
    }
  };

  template <typename T>
  struct mul
  {
    inline static T map(T a, T b)
    {
      return a * b;
    }
  };

  template <typename T>
  struct div
  {
    inline static T map(T a, T b)
    {
      return a / b;
    }
  };

  template <typename Arg, typename T = typename Arg::DataType>
  inline UnaryOp<plus<T>, T, Arg>
  operator+(const Arg& arg)
  {
    return F<plus<T>, T>(arg);
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<plus<T>, T, Lhs, Rhs>
  operator+(const Lhs& lhs, const Rhs& rhs)
  {
    return F<plus<T>, T>(lhs, rhs);
  }

  template <typename Arg, typename T = typename Arg::DataType>
  inline UnaryOp<minus<T>, T, Arg>
  operator-(const Arg& arg)
  {
    return F<minus<T>, T>(arg);
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<minus<T>, T, Lhs, Rhs>
  operator-(const Lhs& lhs, const Rhs& rhs)
  {
    return F<minus<T>, T>(lhs, rhs);
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<mul<T>, T, Lhs, Rhs>
  operator*(const Lhs& lhs, const Rhs& rhs)
  {
    return F<mul<T>, T>(lhs, rhs);
  }

  template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
  inline BinaryOp<div<T>, T, Lhs, Rhs>
  operator/(const Lhs& lhs, const Rhs& rhs)
  {
    return F<div<T>, T>(lhs, rhs);
  }
}

#endif
