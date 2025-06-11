#ifndef SHAPE_H
#define SHAPE_H

#include <array>
#include <stdexcept>
#include <algorithm>

namespace Mathub
{

    template <unsigned N>
    struct Shape
    {
        std::array<unsigned, N> shape = {};
        constexpr unsigned &operator[](unsigned i) { return shape[i]; }
        constexpr const unsigned &operator[](unsigned i) const { return shape[i]; }
    };

    template <>
    struct Shape<0>
    {
        std::array<unsigned, 0> shape = {};
    };

    // C++20 consteval function to calculate broadcasted shape at compile time
    template <unsigned D, unsigned D1, unsigned D2>
    consteval Shape<D> calculate_broadcast_shape(const Shape<D1> &s1, const Shape<D2> &s2)
    {
        if constexpr (D == 0)
            return Shape<0>{};

        Shape<D> result_shape;

        for (unsigned i = 0; i < D; ++i)
        {
            int i1 = static_cast<int>(D1) - 1 - i;
            int i2 = static_cast<int>(D2) - 1 - i;

            unsigned dim1 = (i1 >= 0) ? s1.shape[i1] : 1;
            unsigned dim2 = (i2 >= 0) ? s2.shape[i2] : 1;
            unsigned &out_dim = result_shape.shape[D - 1 - i];

            if (dim1 == dim2)
            {
                out_dim = dim1;
            }
            else if (dim1 == 1)
            {
                out_dim = dim2;
            }
            else if (dim2 == 1)
            {
                out_dim = dim1;
            }
            else
            {
                // This throw will produce a compile-time error
                throw "Incompatible shapes for broadcasting";
            }
        }
        return result_shape;
    }

} // namespace Mathub

#endif