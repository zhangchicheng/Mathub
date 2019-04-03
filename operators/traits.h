#ifndef TRAITS_H
#define TRAITS_H

// minic std::conditional
template <bool B, typename T, typename F>
struct CondType
{
  using type = T;
};

template <typename T, typename F>
struct CondType<false, T, F>
{
  using type = F;
};

template <bool B, unsigned M, unsigned N>
struct CondValue
{
  static constexpr unsigned value = M;
};

template <unsigned M, unsigned N>
struct CondValue<false, M, N>
{
  static constexpr unsigned value = N;
};

template <typename T, typename F>
struct Promote
{
  static constexpr bool B = sizeof(T)>sizeof(F);
  using type = typename CondType<B, T, F>::type;
};

// minic std::integral_constant
template <typename T, T v>
struct Constant
{
  static constexpr T value = v;
};

// check if an object is an instance of a template
template <class, template <class> class>
struct isInstance : public Constant<bool, false> {};

template <class T, template <class> class U>
struct isInstance<U<T>, U> : public Constant<bool, true> {};

template<unsigned N, typename T, typename Func>
class Loop {
  public:
    static inline T eval(Func f){
      return f(N) + Loop<N-1, T, Func>::eval(f);
    }
};

template<typename T, typename Func>
class Loop<0, T, Func>{
  public:
    static inline T eval(Func f){
      return f(0);
    }
};

#endif
