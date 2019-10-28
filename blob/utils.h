#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include "../traits/traits.h"

namespace Mathub
{
template <typename T, unsigned... Ns> struct Array;

template <typename L, typename R, unsigned...I>
void _func(L& _data, R& _expr, IndexSeq<I...>)
{
    ((_data[I] = _expr.eval(I)), ...);
}

template <unsigned I, typename L, typename R>
void op(L& _data, R& _expr)
{
    _func(_data, _expr, MakeIndexSeq<I>());
}

// static variable is not a good choice.
static int indent = 0;
template <typename T, unsigned N, unsigned... Ns>
std::ostream& operator<<(std::ostream& os, const Array<T,N,Ns...>& arr)
{
    os<<'[';

    for (int i=0; i<N; i++)
    {
        indent++;
        os<<arr[i];
        if (i != N-1)
        {
            if (arr.dim==1)
                os<<','<<' ';
            else
            {
                os<<','<<std::endl;
                for (int i=0; i<indent; i++)
                    os<<' ';
            }
        }
        indent--;
    }
    os<<']';
    return os;
}
}

#endif
