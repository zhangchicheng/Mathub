#ifndef OPERATORS_H
#define OPERATORS_H

#include "expression.h"
#include "concepts.h"
#include <type_traits>
#include <utility> // For std::move

namespace Mathub
{
    // ===================================================================
    // THE DEFINITIVE FIX IS HERE
    // The operator tags now have their full, correct definitions with the
    // static map functions that the expression templates rely on.
    // ===================================================================

    // Operation tags
    struct plus
    {
        template <typename T>
        static auto map(T a) { return a; }
        template <typename T1, typename T2>
        static auto map(T1 a, T2 b) { return a + b; }
    };
    struct minus
    {
        template <typename T>
        static auto map(T a) { return -a; }
        template <typename T1, typename T2>
        static auto map(T1 a, T2 b) { return a - b; }
    };
    struct mul
    {
        template <typename T1, typename T2>
        static auto map(T1 a, T2 b) { return a * b; }
    };
    struct div
    {
        template <typename T1, typename T2>
        static auto map(T1 a, T2 b) { return a / b; }
    };

    namespace detail
    {
        // as_expr is now simplified. It always returns an object by value.
        template <WrappableExpr T>
        auto as_expr(const T &t)
        {
            if constexpr (MathubExpr<T>)
            {
                return t.self(); // Returns a copy of the expression object
            }
            else
            {                              // IsMathubArray<T>
                return ArrayWrapper<T>(t); // Returns a new wrapper that copies the array
            }
        }
    }

    // Unary Operators
    template <WrappableExpr Arg>
    auto operator+(const Arg &arg)
    {
        using ExprT = decltype(detail::as_expr(arg));
        return UnaryOp<plus, ExprT>(detail::as_expr(arg));
    }

    template <WrappableExpr Arg>
    auto operator-(const Arg &arg)
    {
        using ExprT = decltype(detail::as_expr(arg));
        return UnaryOp<minus, ExprT>(detail::as_expr(arg));
    }

    // Binary Operators
    template <WrappableExpr Lhs, WrappableExpr Rhs>
    auto operator+(const Lhs &lhs, const Rhs &rhs)
    {
        using LhsExprT = decltype(detail::as_expr(lhs));
        using RhsExprT = decltype(detail::as_expr(rhs));
        return BinaryOp<plus, LhsExprT, RhsExprT>(detail::as_expr(lhs), detail::as_expr(rhs));
    }

    template <WrappableExpr Lhs, WrappableExpr Rhs>
    auto operator-(const Lhs &lhs, const Rhs &rhs)
    {
        using LhsExprT = decltype(detail::as_expr(lhs));
        using RhsExprT = decltype(detail::as_expr(rhs));
        return BinaryOp<minus, LhsExprT, RhsExprT>(detail::as_expr(lhs), detail::as_expr(rhs));
    }

    template <WrappableExpr Lhs, WrappableExpr Rhs>
    auto operator*(const Lhs &lhs, const Rhs &rhs)
    {
        using LhsExprT = decltype(detail::as_expr(lhs));
        using RhsExprT = decltype(detail::as_expr(rhs));
        return BinaryOp<mul, LhsExprT, RhsExprT>(detail::as_expr(lhs), detail::as_expr(rhs));
    }

    template <WrappableExpr Lhs, WrappableExpr Rhs>
    auto operator/(const Lhs &lhs, const Rhs &rhs)
    {
        using LhsExprT = decltype(detail::as_expr(lhs));
        using RhsExprT = decltype(detail::as_expr(rhs));
        return BinaryOp<div, LhsExprT, RhsExprT>(detail::as_expr(lhs), detail::as_expr(rhs));
    }

} // namespace Mathub

#endif