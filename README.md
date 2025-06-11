# **Mathub**

Mathub is a C++ template library for linear algebra.

## **Overview**

* It supports arbitrary array sizes.  
* It supports all standard numeric types and is easily extensible to custom numeric types.  
* It leverages **C++20** features like concepts, consteval, and move semantics for improved safety, compiler diagnostics, and performance.

## **Usage**

Mathub is a pure template library defined in the headers. There is no binary library to link to and no configured header file. To use Mathub, you just need to make the compiler be able to find the header files. Mathub requires a C++20 compatible compiler.

Here is a simple program to get you started.
```C++
#include "mathub/mathub.h"

using namespace Mathub;

int main()  
{  
  Vector<int,3> vec = {2,4,6};
  std::cout << vec << std::endl;  
    
  Matrix<int,2,2> mat = {1,3,5,7};
  Matrix<int,2,1> mat2 = {2,3};  
  Matrix<int,2,2> mat3;  
  mat3 = -mat + mat2 * mat2;  
  std::cout << mat3 << std::endl;  
    
  Tensor<int,2,3,2> arr = {0,1,2,3,4,5,6,7,8,9,0,1}; // or Array<int,2,3,2>  
  std::cout << arr << std::endl;  
}
```
When you run the program, it produces the following output which is similar to `NumPy`:

```
[2, 4, 6]

[[3, 1],  
 [4, 2]]

[[[0, 1],  
  [2, 3],  
  [4, 5]],  
 [[6, 7],  
  [8, 9],  
  [0, 1]]]
```
## **Details**

### **Array**

The core `Array` class is implemented using variadic templates. The C++20 version now uses `std::array` internally for better safety, standard library integration, and features like defaulted comparisons.

```C++
// C++20 version - 1D Array  
template <Numeric T, unsigned N>  
struct Array<T, N>  
{  
    // ...  
    using SubType = T;  
    std::array<SubType, N> data; // Uses std::array  
    // ...  
};

// C++20 version - N-D Array  
template <Numeric T, unsigned N, unsigned... Ns>  
struct Array<T, N, Ns...>  
{  
    // ...  
    using SubType = Array<T, Ns...>;  
    std::array<SubType, N> data; // Uses std::array  
    // ...  
};
```
Accessing elements and sub-arrays remains intuitive. The operator() for element access is now constrained using C++20 concepts for better error messages.
```C++
Tensor<int,2,3,2> arr = {0,1,2,3,4,5,6,7,8,9,0,1};  
std::cout << arr[0] << std::endl;     // Access sub-array  
std::cout << arr(0,2,1) << std::endl; // Access single element

// Implementation of operator()  
template<typename... Indices>  
constexpr T& operator()(unsigned i, Indices... is)  
    requires(sizeof...(Indices) == sizeof...(Ns))  
{  
    return data[i](is...);  
}
```
### **Broadcasting with consteval**

The term broadcasting describes how `NumPy` treats arrays with different shapes during arithmetic operations. Subject to certain constraints, the smaller array is “broadcast” across the larger array so that they have compatible shapes. When operating on two arrays, `NumPy` compares their shapes element-wise. It starts with the trailing dimensions, and works its way forward. Two dimensions are compatible when

1. they are equal, or  
2. one of them is 1

In the following example, both the A and B arrays have axes with length one that are expanded to a larger size during the broadcast operation:
```
A      (4d array):  8 x 1 x 6 x 1  
B      (3d array):      7 x 1 x 5  
Result (4d array):  8 x 7 x 6 x 5
```
The C++17 implementation of broadcasting required complex recursive templates using techniques like static table generation.
```C++
/* C++17 implementation (simplified for illustration).  
  This uses template metaprogramming to calculate the broadcasted shape.  
    
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
                                      
// Specialization for when the first tensor's dimensions are exhausted  
template <unsigned N, unsigned N2, typename S1, typename S2, unsigned ...D>  
struct Broadcasting <N, 0, N2, Lhs, Rhs, D...> : Broadcasting<N - 1, 0, N2 - 1, S1, S2, S2[N2 - 1], D...> {};

// Specialization for when the second tensor's dimensions are exhausted  
template <unsigned N, unsigned N1, typename Lhs, typename Rhs, unsigned ...D>  
struct Broadcasting <N, N1, 0, Lhs, Rhs, D...> : Broadcasting<N - 1, N1 - 1, 0, S1, S2, S1[N1 - 1], D...> {};

// Base case for the recursion  
template <typename S1, typename S2, unsigned ...D>  
struct Broadcasting <0, 0, 0, S1, S2, D...>  
{  
    static constexpr unsigned shape[sizeof...(D)] = {D...};  
};
```
The C++20 version simplifies this dramatically by using a consteval function. This code is executed at compile-time but is written in a much more readable, imperative style, and can produce clear compile-time errors.
```C++
// C++20 implementation using a consteval function  
template<unsigned D, unsigned D1, unsigned D2>  
consteval Shape<D> calculate_broadcast_shape(const Shape<D1>& s1, const Shape<D2>& s2)  
{  
    // ... (imperative logic to check dimensions) ...

    if (dimensions_are_incompatible) {  
        // This throw generates a clear compile-time error  
        throw "Incompatible shapes for broadcasting";   
    }  
      
    // ... (logic to build the result shape) ...  
    return result_shape;  
}
```
### **Expression Templates and Lazy Evaluation**

Mathub uses **expression templates** to avoid creating temporary `Array` objects for intermediate calculations, which is crucial for performance. This is a form of **lazy evaluation**. An expression like mat3 = -mat + mat2 * mat2; does not perform any calculations immediately. Instead, it builds a lightweight expression tree object at compile time.
```C++
template <typename Op, typename Lhs, typename Rhs>  
struct BinaryOp : public Expr<BinaryOp<Op, Lhs, Rhs>>  
{  
    const Lhs lhs; // Stores the left sub-expression  
    const Rhs rhs; // Stores the right sub-expression  
      
    // ...  
};
```
To prevent unnecessary copying of these temporary expression objects as the tree is built, Mathub utilizes **move semantics** in the expression constructors. This allows the compiler to efficiently "move" sub-expressions into new ones, approaching the ideal of zero-cost abstraction.
```C++
// The constructor takes r-value references to move from temporaries  
explicit BinaryOp(Lhs&& lhs, Rhs&& rhs)  
    : lhs(std::move(lhs)), rhs(std::move(rhs)) {}
```
The actual element-wise computation only happens when the expression is finally assigned to a concrete Array, such as mat3.