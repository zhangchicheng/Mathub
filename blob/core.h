#ifndef CORE_H
#define CORE_H

#include "../expression/expr.h"
#include "./shape.h"

namespace Mathub
{
  template <typename SubType> struct Expr;
  template <unsigned N> struct Shape;

  template <typename T, unsigned... Ns> struct mathub {};

  template <typename T>
  struct mathub<T>
  {
    using DataType = T;
    using Type = T;
    Type data;

    static constexpr unsigned dim = 0;
    static constexpr Shape<0> shape = {0};

    template<typename SubType>
    inline mathub& operator=(const Expr<SubType>& expr_)
    {
      const SubType & expr = expr_.self();
      data = expr.eval();
      return *this;
    }

    inline T eval() const
    {
      return data;
    }
    inline const mathub& self() const
    {
      return *this;
    }
  };

  template <typename T, unsigned N>
  struct mathub<T, N>
  {
    using DataType = T;
    using Type = T[N];
    Type data;

    static constexpr unsigned dim = 1;
    static constexpr Shape<dim> shape = {N};

    T& operator[](unsigned i) { return data[i]; }
    const T& operator[](unsigned i) const { return data[i]; }

    T& operator()(unsigned i) { return data[i]; }
    const T& operator()(unsigned i) const { return data[i]; }

    template<typename SubType>
    inline mathub& operator=(const Expr<SubType>& expr_)
    {
      const SubType &expr = expr_.self();
      for (unsigned i = 0; i < N; ++i)
      {
        data[i] = expr.eval(i);
      }
      return *this;
    }

    inline T eval(unsigned i) const
    {
      return data[i];
    }
    inline const mathub& self() const
    {
      return *this;
    }
  };

  template <typename T, unsigned N1, unsigned N2>
  struct mathub<T, N1, N2>
  {
    using DataType = T;
    using SubType = mathub<T, N2>;
    using Type = SubType[N1];
    Type data;

    static constexpr unsigned dim = 2;
    static constexpr Shape<dim> shape = {N1, N2};

    SubType& operator[](unsigned i) { return data[i]; }
    const SubType& operator[](unsigned i) const { return data[i]; }

    T& operator()(unsigned i, unsigned j) { return data[i][j]; }
    const T& operator()(unsigned i, unsigned j) const { return data[i][j]; }

    template<typename SubType>
    inline mathub& operator=(const Expr<SubType>& expr_)
    {
      const SubType &expr = expr_.self();
      for (unsigned i = 0; i < N1; ++i)
      {
        for (unsigned j = 0; j < N2; ++j)
        {
          data[i][j] = expr.eval(i,j);
        }
      }
      return *this;
    }

    inline T eval(unsigned i, unsigned j) const {
      return data[i][j];
    }
    inline const mathub& self() const
    {
      return *this;
    }

  };

  template <typename T, unsigned N, unsigned... Ns>
  struct mathub<T, N, Ns...>
  {
    using DataType = T;
    using SubType = mathub<T, Ns...>;
    using Type = SubType[N];
    Type data;

    static constexpr unsigned dim = sizeof...(Ns) + 1;
    static constexpr Shape<dim> shape = {N, Ns...};

    SubType& operator[](unsigned i) { return data[i]; }
    const SubType& operator[](unsigned i) const { return data[i]; }

    T& operator()(unsigned i, decltype(Ns)... is) { return data[i](is...); }
    const T& operator()(unsigned i, decltype(Ns)... is) const { return data[i](is...); }
  };

  template <typename T>
  using scalar = mathub<T>;

  template <typename T, unsigned N1>
  using vector = mathub<T, N1>;

  template <typename T, unsigned N1, unsigned N2>
  using matrix = mathub<T, N1, N2>;

  template <typename T, unsigned N1, unsigned N2, unsigned N3>
  using tensor = mathub<T, N1, N2, N3>;
}

#endif
