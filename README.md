# Mathub
Mathub is a C++ template library for linear algebra.

## Overview
* It supports arbitrary array sizes.
* It supports all standard numeric types and is easily extensible to custom numeric types.
* It does not have any dependencies other than iostream.

## Usage
Mathub is a pure template library defined in the headers. There is no binary library to link to, and no configured header file. In order to use Mathub, you just need to make the compiler be able to find the header files. Mathub requires a modern C++ compiler supporting C++17.
Here is a simple program to get you started.

    #include "mathub.h"
    
    using namespace Mathub;
    
    int main()
    {
      Vector<int,3> vec = {2,4,6}; // or Array<int,3>
      std::cout << vec << std::endl;
      
      Matrix<int,2,2> mat = {1,3,5,7}; // or Array<int,2,2>
      Matrix<int,2,1> mat2 = {2,3};
      Matrix<int,2,2> mat3;
      mat3 = -mat + mat2 * mat2;
      std::cout << mat3 << std::endl;
      
      Tensor<int,2,3,2> arr = {0,1,2,3,4,5,6,7,8,9,0,1}; // or Array<int,2,3,2>
      std::cout << arr << std::endl;
    }
When you run the program, it produces the following output which is similar to NumPy:
    
    [2, 4, 6]

    [[3, 1],
     [4, 2]]

    [[[0, 1],
      [2, 3],
      [4, 5]],
     [[6, 7],
      [8, 9],
      [0, 1]]]

## Details
* Array

    The `Array` is implemented by using variadic template.

        template <typename T, unsigned... Ns> struct Array {};
    
        template <typename T, unsigned N>
        struct Array<T, N>
        {
            ...
      
            using SubType = T;
            using Type = SubType[N];
            Type data;
      
            ...
        };

        template <typename T, unsigned N, unsigned... Ns>
        struct Array<T, N, Ns...>
        {
            ...
        
            using SubType = Array<T, Ns...>;
            using Type = SubType[N];
            Type data;
        
            ...
        };
    
    Besides, variadic template is widely used in this project such as parenthesis operator overloading and output operator overloading

        Tensor<int,2,3,2> arr = {0,1,2,3,4,5,6,7,8,9,0,1};
        std::cout<< arr[0] << std::endl; // access sub array
        std::cout<< arr(0,2,1) <<std::endl // access single element
    
        [[0, 1],
         [2, 3],
         [4, 5]]
     
        5
    Inplementation:

        template <typename T, unsigned N, unsigned... Ns>
        struct Array<T, N, Ns...>
        {
            ...
            constexpr T& operator()(unsigned i, decltype(Ns)... is) { return data[i](is...); }
            ...
        }
    
* Broadcasting:
    > The term broadcasting describes how numpy treats arrays with different shapes during arithmetic operations. Subject to certain constraints, the smaller array is “broadcast” across the larger array so that they have compatible shapes.
    > When operating on two arrays, NumPy compares their shapes element-wise. It starts with the trailing dimensions, and works its way forward. Two dimensions are compatible when
    >   1. they are equal, or
    >   2. one of them is 1

    >   In the following example, both the A and B arrays have axes with length one that are expanded to a larger size during the broadcast operation:

        A      (4d array):  8 x 1 x 6 x 1
        B      (3d array):      7 x 1 x 5
        Result (4d array):  8 x 7 x 6 x 5

    The implementation of broadcasting is a little tricky which use a technique called [static table generation](https://en.wikipedia.org/wiki/Template_metaprogramming#Static_Table_Generation).
    
        /* For the sake of simplicity, the code was simplified and not applicable directly.
           N : dimension of output
           N1: dimension of first tensor
           N2: dimension of second tensor
           S1: shape of first tensor
           S2: shape of second tensor
        */
        template <unsigned N, unsigned N1, unsigned N2, typename S1, typename S2, unsigned ...D>
        struct Broadcasting : Broadcasting<N - 1, N1 - 1, N2 - 1, Lhs, Rhs,
                                            (S1[N1 - 1] == S2[N2 - 1]) ? 
                                            S1[N1-1] : 
                                            (S1[N1 - 1] == 1 or S2[N2 - 1] == 1) ? 
                                            S1[N1 - 1] * S2[N2 - 1] : error,
                                            D...> {};
                                            
        // specialization                                    
        template <unsigned N, unsigned N2, typename S1, typename S2, unsigned ...D>
        struct Broadcasting <N, 0, N2, Lhs, Rhs, D...> : Broadcasting<N - 1, 0, N2 - 1, S1, S2, S2[N2 - 1], D...> {};

        template <unsigned N, unsigned N1, typename Lhs, typename Rhs, unsigned ...D>
        struct Broadcasting <N, N1, 0, Lhs, Rhs, D...> : Broadcasting<N - 1, N1 - 1, 0, S1, S2, S1[N1 - 1], D...> {};

        template <typename S1, typename S2, unsigned ...D>
        struct Broadcasting <0, 0, 0, S1, S2, D...>
        {
            static constexpr unsigned shape[sizeof...(D)] = {D...};
        };


* Expression Template and Lazy Evaluation
