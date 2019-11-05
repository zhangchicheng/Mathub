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
    std::cout << arr(1,1,0) << std::endl;
    std::cout << arr[1][1][0] << std::endl;
    std::cout << arr[1][0].dim << std::endl;
    std::cout << arr.shape[2] << std::endl;

    Tensor<int, 2,2,3> arr1 = {1,2,3,2,1,2,3,4,5,7,8,9};
    Tensor<int, 2,1,3> arr2 = {0,1,4,1,2,3};
    Tensor<int, 2,2,3> arr3 = {0,0,0,0,0,0,0,0,0,0,0,0};
    arr3 = arr1+arr2;
    std::cout<<arr3<<std::endl;
}
