#ifndef OPS_H
#define OPS_H

#include "../traits/traits.h"
#include<iostream>
namespace Mathub
{
template <typename Op, typename T, typename Arg> struct UnaryOp;
template <typename Op, typename Lhs, typename Rhs> struct BinaryOp;

template <typename Op, typename T, typename Arg>
inline UnaryOp<Op, T, Arg> F(const Arg& arg);
template <typename Op, typename T, typename Lhs, typename Rhs>
inline BinaryOp<Op, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs);

template <typename Lhs, typename Rhs>
struct plus
{
    //inline static auto map(T a) { return a; }
    inline static auto map(Lhs a, Rhs b) { return a + b; }
};

template <typename T>
struct minus
{
    inline static auto map(T a) { return -a; }
    inline static auto map(T a, T b) { return a - b; }
};

template <typename T>
struct mul
{
    inline static auto map(T a, T b) { return a * b; }
};

template <typename T>
struct div
{
    inline static auto map(T a, T b) { return a / b; }
};

/*
template <typename Arg, typename T = typename Arg::DataType>
inline UnaryOp<plus<T>, T, Arg> operator+(const Arg& arg)
{
    return F<plus<T>, T>(arg);
}
 */

template <typename Arg, typename T = typename Arg::DataType>
inline UnaryOp<minus<T>, T, Arg> operator-(const Arg& arg)
{
    return F<minus<T>, T>(arg);
}

template <typename Lhs, typename Rhs>
inline BinaryOp<plus<Lhs, Rhs>, Lhs, Rhs> operator+(const Lhs& lhs, const Rhs& rhs)
{
    return F<plus<Lhs, Rhs>>(lhs, rhs);
}
/*
template <typename Lhs, typename Rhs, typename T = typename Lhs::SubType>
inline BinaryOp<minus<T>, Lhs, Rhs> operator-(const Lhs& lhs, const Rhs& rhs)
{
    return F<minus<T>>(lhs, rhs);
}

template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
inline BinaryOp<mul<T>, Lhs, Rhs> operator*(const Lhs& lhs, const Rhs& rhs)
{
    return F<mul<T>, T>(lhs, rhs);
}

template <typename Lhs, typename Rhs, typename T = typename Promote<typename Lhs::DataType, typename Rhs::DataType>::type>
inline BinaryOp<div<T>, Lhs, Rhs> operator/(const Lhs& lhs, const Rhs& rhs)
{
    return F<div<T>, T>(lhs, rhs);
}
 */

}

#endif
