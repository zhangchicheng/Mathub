#include "mathub.h"
#include<iostream>

using namespace Mathub;

int main()
{
    Array<int, 2,1,2> m0 = {1,2,3,4};
    std::cout << m0 << std::endl;

    Tensor<int, 2,3,2> m1 = {1,3,5,7,9,2,4,6,8,1,2,4};
    std::cout << m1[1][1][0] << std::endl;
    std::cout << m1[1][0].dim << std::endl;
    std::cout << m1.shape[2] << std::endl;
    std::cout << m1 << std::endl;

    Matrix<int,2,1> m2 = {2,3};
    std::cout << m2 << std::endl;

    std::cout<<m1(1,0,0)<<std::endl;

    Matrix <int,1,3> v4 = {1,2,3};
    Matrix <int,1,3> v5 = {2,1,2};
    Matrix <int,1,3> v6 = {0,0,0};
    v6 = v4+v5;
    std::cout<<v6[0]<<std::endl;
    Matrix<int, 2,3> v0 = {1,2,3,2,1,2};
    Matrix<int, 2,3> v1 = {0,1,4,2,2,4};
    Matrix<int, 2,3> v2 = {0,0,0,0,0,0};
    v2 = v0+v4+v1;
    std::cout<<v2[1]<<std::endl;

    Tensor<int, 2,2,3> t0 = {1,2,3,2,1,2,3,4,5,7,8,9};
    Tensor<int, 2,1,3> t1 = {0,1,4,1,2,3};
    //Tensor<int, 2,2,3> t3 = {0,0,0,0,0,0,0,0,0,0,0,0};
    Tensor<int, 2,2,3> t3;
    t3 = t0+t1;
    std::cout<<t3<<std::endl;
    //t3 = t0 + v0;
    //std::cout<<t0<<std::endl;
    //constexpr Shape<3> s = {1,2,3};
    //constexpr auto arr = genArr<int,3,s>::arr;
    //std::cout<<arr[0]<<std::endl;
}
