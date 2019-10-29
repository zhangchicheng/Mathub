#ifndef OPS_H
#define OPS_H

#include "../traits/traits.h"
#include<iostream>
namespace Mathub
{
template <typename Op, typename Arg> struct UnaryOp;
template <typename Op, typename Lhs, typename Rhs> struct BinaryOp;

template <typename Op, typename T, typename Arg>
inline UnaryOp<Op, Arg> F(const Arg& arg);

template <typename Op, typename T, typename Lhs, typename Rhs>
inline BinaryOp<Op, Lhs, Rhs> F(const Lhs& lhs, const Rhs& rhs);

struct plus {};
struct minus {};
struct mul {};
struct div {};

template <typename Arg>
inline UnaryOp<plus, Arg> operator+(const Arg& arg)
{
    return F<plus>(arg);
}

template <typename Arg>
inline UnaryOp<minus, Arg> operator-(const Arg& arg)
{
    return F<minus>(arg);
}

template <typename Lhs, typename Rhs>
inline BinaryOp<plus, Lhs, Rhs> operator+(const Lhs& lhs, const Rhs& rhs)
{
    return F<plus>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
inline BinaryOp<minus, Lhs, Rhs> operator-(const Lhs& lhs, const Rhs& rhs)
{
    return F<minus>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
inline BinaryOp<mul, Lhs, Rhs> operator*(const Lhs& lhs, const Rhs& rhs)
{
    return F<mul>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
inline BinaryOp<div, Lhs, Rhs> operator/(const Lhs& lhs, const Rhs& rhs)
{
    return F<div>(lhs, rhs);
}

}

#endif
