#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>
#include <type_traits>

namespace Mathub
{

    // Forward declarations to break circular dependencies
    template <typename SubType>
    struct Expr;
    template <typename T, unsigned... Ns>
    struct Array;

    // Concept for any type that can be used in arithmetic operations
    template <typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    // Concept to identify any type that derives from Expr<T>
    template <typename T>
    concept MathubExpr = std::is_base_of_v<Expr<T>, T>;

    // Type trait and concept to identify our Array class
    template <typename T>
    struct is_mathub_array : std::false_type
    {
    };
    template <Numeric T, unsigned... Ns>
    struct is_mathub_array<Array<T, Ns...>> : std::true_type
    {
    };
    template <typename T>
    inline constexpr bool is_mathub_array_v = is_mathub_array<std::remove_cvref_t<T>>::value;
    template <typename T>
    concept IsMathubArray = is_mathub_array_v<T>;

    // Concept for any type that can participate in an expression (Array or Expr)
    template <typename T>
    concept WrappableExpr = MathubExpr<T> || IsMathubArray<T>;

} // namespace Mathub

#endif