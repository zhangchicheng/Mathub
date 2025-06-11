#include "mathub/mathub.h"
#include <iostream>

using namespace Mathub;

int main()
{
    Vector<int, 3> vec = {2, 4, 6}; // or Array<int,3>
    std::cout << "vec:\n"
              << vec << std::endl
              << std::endl;

    Matrix<int, 2, 2> mat = {1, 3, 5, 7};
    Matrix<int, 2, 1> mat2 = {2, 3};
    Matrix<int, 2, 2> mat3;

    std::cout << "\n--- STARTING mat3 calculation ---\n"
              << std::endl;
    {
        // Create the expression tree
        auto expr_tree = -mat + mat2 * mat2;
        std::cout << "\n--- Expression tree CREATED, starting assignment to mat3 ---\n"
                  << std::endl;

        // Assign the result
        mat3 = expr_tree;

        std::cout << "\n--- Assignment to mat3 FINISHED ---\n"
                  << std::endl;
    } // expr_tree goes out of scope and is destroyed here

    std::cout << "\n--- READY to print mat3 ---\n"
              << std::endl;
    std::cout << "mat3 = -mat + mat2 * mat2:\n"
              << mat3 << std::endl
              << std::endl;

    Tensor<int, 2, 3, 2> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1}; // or Array<int,2,3,2>
    std::cout << "Tensor arr:\n"
              << arr << std::endl
              << std::endl;

    std::cout << "arr(1,1,0): " << arr(1, 1, 0) << std::endl;
    std::cout << "arr[1][1]: " << arr[1][1] << std::endl; // Changed from arr[1][1][0]
    std::cout << "Dimension of arr[1]: " << arr[1].dim << std::endl;
    std::cout << "Third dimension size of arr: " << arr.shape[2] << std::endl
              << std::endl;

    // Broadcasting example 1
    Tensor<int, 2, 2, 3> arr1 = {1, 2, 3, 2, 1, 2, 3, 4, 5, 7, 8, 9};
    Tensor<int, 2, 1, 3> arr2 = {0, 1, 4, 1, 2, 3};
    // This now works because operator<< is defined for the BinaryOp result
    std::cout << "arr1 + arr2 (broadcasting):\n"
              << (arr1 + arr2) << std::endl
              << std::endl;

    // Broadcasting example 2
    Matrix<int, 2, 3> m = {1, 2, 3, 4, 5, 6};
    std::cout << "arr1 + m (broadcasting):\n"
              << (arr1 + m) << std::endl;
}