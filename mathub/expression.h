#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <algorithm>
#include <type_traits>
#include "concepts.h"
#include "shape.h"

namespace Mathub
{

    template <typename SubType>
    struct Expr
    {
        const SubType &self() const
        {
            return *static_cast<const SubType *>(this);
        }
    };

    // ArrayWrapper now stores the array by value. This is only used for lvalues, so it's a copy.
    template <IsMathubArray T>
    struct ArrayWrapper : public Expr<ArrayWrapper<T>>
    {
        const T array_obj; // Changed from reference to value

        using DataType = typename T::DataType;
        static constexpr unsigned dim = T::dim;
        static constexpr auto shape = T::shape;

        explicit ArrayWrapper(const T &ref) : array_obj(ref) {}

        auto eval(unsigned i) const
        {
            return array_obj.eval(i);
        }
    };

    template <typename Op, MathubExpr Arg>
    struct UnaryOp : public Expr<UnaryOp<Op, Arg>>
    {
        const Arg arg; // Stored by value, prevents dangling

        using DataType = typename Arg::DataType;
        static constexpr unsigned dim = Arg::dim;
        static constexpr Shape<dim> shape = Arg::shape;

        explicit UnaryOp(Arg &&arg) : arg(std::move(arg)) {} // Take by rvalue-reference

        auto eval(unsigned i) const
        {
            return Op::map(arg.eval(i));
        }
    };

    template <typename Op, MathubExpr Lhs, MathubExpr Rhs>
    struct BinaryOp : public Expr<BinaryOp<Op, Lhs, Rhs>>
    {
        const Lhs lhs; // Stored by value
        const Rhs rhs; // Stored by value

        using DataType = std::common_type_t<typename Lhs::DataType, typename Rhs::DataType>;
        static constexpr unsigned dim = std::max(Lhs::dim, Rhs::dim);
        static constexpr Shape<dim> shape = calculate_broadcast_shape<dim>(Lhs::shape, Rhs::shape);

        explicit BinaryOp(Lhs &&lhs, Rhs &&rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)) {} // Take by rvalue-reference

        auto eval(unsigned i) const
        {
            if constexpr (Lhs::dim == Rhs::dim)
            {
                if constexpr (Lhs::shape.shape[0] == Rhs::shape.shape[0])
                    return Op::map(lhs.eval(i), rhs.eval(i));
                else if constexpr (Lhs::shape.shape[0] == 1)
                    return Op::map(lhs.eval(0), rhs.eval(i));
                else
                    return Op::map(lhs.eval(i), rhs.eval(0));
            }
            else if constexpr (Lhs::dim > Rhs::dim)
            {
                return Op::map(lhs.eval(i), rhs);
            }
            else
            {
                return Op::map(lhs, rhs.eval(i));
            }
        }
    };

} // namespace Mathub

#endif