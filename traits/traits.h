#ifndef TRAITS_H
#define TRAITS_H

namespace Mathub
{
template <unsigned... I>
struct IndexSeq {};

template <unsigned N, unsigned ...I>
struct MakeIndexSeq : MakeIndexSeq<N-1, N-1, I...> {};

template <unsigned... I>
struct MakeIndexSeq<0, I...> : IndexSeq<I...> {};

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

template <typename, typename >
struct IsSame
{
    static constexpr bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
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

template<class T>
struct precision_trait {
    enum { precisionRank = 0,
           knowPrecisionRank = 0 };
};

#define DECLARE_PRECISION(T,rank) \
    template<> \
    struct precision_trait<T> \
{ \
    enum { precisionRank = rank}; \
};

DECLARE_PRECISION(int,100)
DECLARE_PRECISION(unsigned int,200)
DECLARE_PRECISION(long,300)
DECLARE_PRECISION(unsigned long,400)
DECLARE_PRECISION(float,500)
DECLARE_PRECISION(double,600)
DECLARE_PRECISION(long double,700)

#ifdef BZ_HAVE_COMPLEX
BZ_DECLARE_PRECISION(complex<float>,800)
BZ_DECLARE_PRECISION(complex<double>,900)
BZ_DECLARE_PRECISION(complex<long double>,1000)
#endif


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


// check if a class support bracket operator
template <typename T>
class hasBracket
{
    typedef char one;
    typedef long two;
    template <typename C> static one test(decltype(&C::data));
    template <typename C> static two test(...);
public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};
}

#endif
