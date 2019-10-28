#ifndef CORE_H
#define CORE_H

#include "../expression/expr.h"
#include "./shape.h"
#include "./utils.h"

namespace Mathub
{
template <typename SubType> struct Expr;
template <unsigned N> struct Shape;

template <typename T, unsigned... Ns> struct Array {};

template <typename T, unsigned N>
struct Array<T, N>
{
    using DataType = T;
    using SubType = T;
    using Type = SubType[N];
    Type data;

    static constexpr unsigned dim = 1;
    static constexpr Shape<dim> shape = {N};

    constexpr SubType& operator[](unsigned i) { return data[i]; }
    constexpr const SubType& operator[](unsigned i) const { return data[i]; }

    constexpr T& operator()(unsigned i) { return data[i]; }
    constexpr const T& operator()(unsigned i) const { return data[i]; }

    template<typename SubType>
    constexpr Array& operator=(const Expr<SubType>& expr)
    {
        op<N>(data, expr.self());
        return *this;
    }

    inline SubType eval(unsigned i) const
    {
        return data[i];
    }

    inline const Array& self() const
    {
        return *this;
    }
};

template <typename T, unsigned N, unsigned... Ns>
struct Array<T, N, Ns...>
{
    using DataType = T;
    using SubType = Array<T, Ns...>;
    using Type = SubType[N];
    Type data;

    static constexpr unsigned dim = sizeof...(Ns) + 1;
    static constexpr Shape<dim> shape = {N, Ns...};

    constexpr SubType& operator[](unsigned i) { return data[i]; }
    constexpr const SubType& operator[](unsigned i) const { return data[i]; }

    constexpr T& operator()(unsigned i, decltype(Ns)... is) { return data[i](is...); }
    constexpr const T& operator()(unsigned i, decltype(Ns)... is) const { return data[i](is...); }

    template <typename SubType>
    inline Array& operator=(const Expr<SubType>& expr)
    {
        op<N>(data, expr.self());
        return *this;
    }

    inline SubType eval(unsigned i) const
    {
        return data[i];
    }

    inline const Array& self() const
    {
        return *this;
    }
};

template <typename T, unsigned N1>
using Vector = Array<T, N1>;

template <typename T, unsigned N1, unsigned N2>
using Matrix = Array<T, N1, N2>;

template <typename T, unsigned N1, unsigned N2, unsigned N3>
using Tensor = Array<T, N1, N2, N3>;

}

#endif
