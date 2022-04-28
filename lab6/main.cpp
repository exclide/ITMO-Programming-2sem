#include <iostream>
#include <array>
#include <type_traits>
#define GOOGLE_TEST 1

#include "Constexpr.h"
#include "gTests.h"


int main() {
    Polynom<1, 2, 3> poly1; //x^2 + 2x + 3
    Polynom<2, 0, 1, 4> poly2; //2x^3 + x + 4
    Polynom<3, -1, 4, 0> poly3; //3x^3 - x^2 + 4x;
    Polynom<-2, -5, 3, 0, 2, -1, 4, 0, -1> poly4; //-2x^8 -5x^7 + 3x^6 + 2x^4 -x^3 + 4x2 - 1
    std::vector<int> vals;
    constexpr int val1 = poly1.GetAt(5); //5^2 + 10 + 3 = 38
    constexpr int val2 = poly2.GetAt(2); //2 * 2^3 + 2 + 4 = 22
    constexpr int val3 = poly3.GetAt(0); //0
    constexpr int val4 = poly4.GetAt(-5); //-342276

    std::cout << poly1 << " = " << val1 << "\n";
    std::cout << poly2 << " = " << val2 << "\n";
    std::cout << poly3 << " = " << val3 << "\n";
    std::cout << poly4 << " = " << val4 << "\n";

#if GOOGLE_TEST == 1
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif
}
