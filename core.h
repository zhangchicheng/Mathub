/*
To do:
    ease of use: arbitrary dimensional array, operator(comma) overloading, *slice
    efficiency: expression template, sparse matrix, multithread
    algorithms:

*/

template<typename T, unsigned... Ns> struct Mathub {};

template<typename T, unsigned N>
struct Mathub<T, N>
{
  using type = T[N];
  type data;
  T& operator[](unsigned i) { return data[i]; }
  const T& operator[](unsigned i) const { return data[i]; }
  constexpr static unsigned size[] = {N};
};

template<typename T, unsigned N, unsigned... Ns>
struct Mathub<T, N, Ns...>
{
  using subMathub = typename Mathub<T, Ns...>::type;
  using type = subMathub[N];
  type data;
  subMathub& operator[](unsigned i) { return data[i]; }
  const subMathub& operator[](unsigned i) const { return data[i]; }
  constexpr static unsigned size[] = {N, Ns...};
};

template<typename T>
using Scalar = T;

template<typename T, unsigned N1>
using Vector = Mathub<T, N1>;

template<typename T, unsigned N1, unsigned N2>
using Matrix = Mathub<T, N1, N2>;

template<typename T, unsigned N1, unsigned N2, unsigned N3>
using Tensor = Mathub<T, N1, N2, N3>;
