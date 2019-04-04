#ifndef TRAITS_H
#define TRAITS_H

namespace Mathub
{
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
}

#endif
