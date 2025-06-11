#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <utility>
#include "concepts.h"

namespace Mathub
{

    // Helper to evaluate an expression and assign it to an array
    template <typename L, typename R, std::size_t... I>
    constexpr void assign_impl(L &array, const R &expr, std::index_sequence<I...>)
    {
        // This line is the heart of the evaluation for assignment
        ((array[I] = expr.eval(I)), ...);
    }

    template <unsigned N, typename L, typename R>
    constexpr void assign(L &array, const R &expr)
    {
        assign_impl(array, expr, std::make_index_sequence<N>());
    }

    // Private recursive helper for thread-safe printing of concrete Arrays
    template <typename T, unsigned N, unsigned... Ns>
    void print_array(std::ostream &os, const Array<T, N, Ns...> &arr, int indent)
    {
        os << '[';
        for (int i = 0; i < N; ++i)
        {
            if constexpr (sizeof...(Ns) > 0)
            {
                if (i > 0)
                {
                    os << ',' << std::endl;
                    for (int j = 0; j < indent + 1; ++j)
                        os << ' ';
                }
                print_array(os, arr[i], indent + 1);
            }
            else
            {
                os << arr[i];
                if (i != N - 1)
                {
                    os << ", ";
                }
            }
        }
        os << ']';
    }

    // Public operator<< for all concrete Array types
    template <Numeric T, unsigned... Ns>
    std::ostream &operator<<(std::ostream &os, const Array<T, Ns...> &arr)
    {
        print_array(os, arr, 0);
        return os;
    }

    namespace detail
    {
        // Helper to get the full Array type from an expression type
        template <typename ExprType, typename Seq>
        struct MakeArrayType;

        template <typename ExprType, std::size_t... Is>
        struct MakeArrayType<ExprType, std::index_sequence<Is...>>
        {
            using Type = Array<typename ExprType::DataType, ExprType::shape.shape[Is]...>;
        };
    } // namespace detail

    // Operator<< for any expression template object
    template <MathubExpr E>
    std::ostream &operator<<(std::ostream &os, const E &expr)
    {
        // Deduce the concrete Array type from the expression's properties
        using ResultArray = typename detail::MakeArrayType<E, std::make_index_sequence<E::dim>>::Type;

        // Create a temporary concrete Array and evaluate the expression into it
        ResultArray temp;
        temp = expr;

        // Print the concrete temporary array using the existing overload
        os << temp;

        return os;
    }

} // namespace Mathub

#endif