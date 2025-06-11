#ifndef CORE_H
#define CORE_H

#include <array>
#include <compare>
#include "concepts.h"
#include "expression.h"
#include "shape.h"
#include "utils.h"

namespace Mathub
{

    // Base case: 1D Array (Vector)
    template <Numeric T, unsigned N>
    struct Array<T, N>
    {
        using DataType = T;
        using SubType = T;
        std::array<SubType, N> data;

        static constexpr unsigned dim = 1;
        static constexpr Shape<dim> shape = {N};

        constexpr SubType &operator[](unsigned i) { return data[i]; }
        constexpr const SubType &operator[](unsigned i) const { return data[i]; }

        constexpr T &operator()(unsigned i) { return data[i]; }
        constexpr const T &operator()(unsigned i) const { return data[i]; }

        auto operator<=>(const Array<T, N> &) const = default;

        // Assignment from any Mathub expression
        template <typename E>
        constexpr Array &operator=(const Expr<E> &expr)
        {
            // *** THE FIX: Explicitly provide N to the assign function ***
            assign<N>(*this, expr.self());
            return *this;
        }

        constexpr SubType eval(unsigned i) const
        {
            return data[i];
        }
    };

    // Recursive case: ND Array
    template <Numeric T, unsigned N, unsigned... Ns>
    struct Array<T, N, Ns...>
    {
        using DataType = T;
        using SubType = Array<T, Ns...>;
        std::array<SubType, N> data;

        static constexpr unsigned dim = sizeof...(Ns) + 1;
        static constexpr Shape<dim> shape = {N, Ns...};

        auto operator<=>(const Array<T, N, Ns...> &) const = default;

        constexpr SubType &operator[](unsigned i) { return data[i]; }
        constexpr const SubType &operator[](unsigned i) const { return data[i]; }

        template <typename... Indices>
        constexpr T &operator()(unsigned i, Indices... is)
            requires(sizeof...(Indices) == sizeof...(Ns))
        {
            return data[i](is...);
        }

        template <typename... Indices>
        constexpr const T &operator()(unsigned i, Indices... is) const
            requires(sizeof...(Indices) == sizeof...(Ns))
        {
            return data[i](is...);
        }

        template <typename E>
        constexpr Array &operator=(const Expr<E> &expr)
        {
            // *** THE FIX: Explicitly provide N to the assign function ***
            assign<N>(*this, expr.self());
            return *this;
        }

        constexpr SubType eval(unsigned i) const
        {
            return data[i];
        }
    };

    // CTAD guide for creating 1D arrays from initializer lists
    template <typename T, typename... U>
    Array(T, U...) -> Array<std::common_type_t<T, U...>, 1 + sizeof...(U)>;

    // Type Aliases
    template <Numeric T, unsigned N1>
    using Vector = Array<T, N1>;
    template <Numeric T, unsigned N1, unsigned N2>
    using Matrix = Array<T, N1, N2>;
    template <Numeric T, unsigned... Dims>
    using Tensor = Array<T, Dims...>;

} // namespace Mathub

#endif