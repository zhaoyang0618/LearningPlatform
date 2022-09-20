#include "testlang.h"
#include <iostream>
#include <cmath>
#include <thread>
#include <syncstream>

//Í¬²½Êä³ö
void squareRoots(int num)
{
    for (int i = 0; i < num; ++i) {
        std::osyncstream coutSync{ std::cout };
        coutSync << "squareroot of " << i << " is "
            << std::sqrt(i) << '\n';
    }
}

void testAsyncStream()
{
    std::jthread t1(squareRoots, 5);
    std::jthread t2(squareRoots, 5);
    std::jthread t3(squareRoots, 5);
}
