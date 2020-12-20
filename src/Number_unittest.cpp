#include <iostream>

#include "Number.h"

template<class T>
void ASSERT_EQ(const T& obj1, const T& obj2, const char* testName) {
    std::cout << "\033[97m" << testName << " Test\033[0m" << std::endl;
    std::cout << "Number 1 = " << obj1 << ", Number 2 = " << obj2 << std::endl;
    if (obj1 == obj2) {
        std::cout << "\033[92m[ PASSED ]\033[0m" << std::endl;
    }
    else {
        std::cout << "\033[91m[ FAILED ]\033[0m" << std::endl;
    }
}

void constructorTests() {
    std::cout << "\033[96mConstructor Tests\033[0m" << std::endl;
    Number num = Number(2e5);
    ASSERT_EQ(num, Number(1, log10(2e5), 1), "Value Constructor");

    num = Number(1, 2, 1);
    ASSERT_EQ(num, Number(100), "Default Constructor");

    num = Number(1, 6);
    ASSERT_EQ(num, Number(1, 6, 1), "Scientific Constructor");

    num = Number(7, 6, -1);
    Number num2 = Number(num);
    ASSERT_EQ(num2, num, "Copy Constructor");
}

void basicMathTests() {
    std::cout << "\033[96mBasic Math Tests\033[0m" << std::endl;
    Number num = Number(1) + Number(.5); // 1 + .5 = 1.5
    num += Number(150); // 1.5 + 150 = 151.5
    ASSERT_EQ(num, Number(151.5), "Addition");

    num += Number(-200); // 151.5 - 200 = -48.5
    ASSERT_EQ(num, Number(-48.5), "Negative Addition");

    num = Number(200) - Number(30); // 200 - 30 = 170
    num -= Number(5); // 170 - 5 = 165
    ASSERT_EQ(num, Number(165, 0), "Subtraction");

    num -= Number(200); // 165 - 200 = -35
    ASSERT_EQ(num, Number(-35), "Negative Subtraction");

    num = Number(4) * Number(-1, 1, 1); // 4 * .1 = .4
    num *= Number(160); // .4 * 160 = 64
    ASSERT_EQ(num, Number(64), "Multiplication");

    num *= Number(-.025); // 64 * -.025 = -1.6
    ASSERT_EQ(num, Number(-1.6), "Negative Multiplication");

    num = Number(1000) / Number(50); // 1000 / 50 = 20
    num /= Number(2.5); // 20 / 2.5 = 8
    ASSERT_EQ(num, Number(8), "Division");

    num /= Number(-2); // 8 / -2 = -4
    ASSERT_EQ(num, Number(-4), "Negative Division");
}

void complexMathTests() {
    std::cout << "\033[96mComplex Math Tests\033[0m" << std::endl;
    Number num = Number(30) ^ 3; // 30 ^ 3 = 27000
    ASSERT_EQ(num, Number(2.7, 4), "Exponentiation");

    num *= -1; // -27000
    num ^= .6; // -27000 ^ .6 = -455.8461157
    ASSERT_EQ(num, Number(-pow(27000, .6)), "Decimal Exponentiation");
    num ^= 1 / .6; // -27000 ^ .6 ^ 1/.6 = -27000
    ASSERT_EQ(num, Number(-27000), "Reverse Exponentiation");

    num = Number(1953125).logBase(5); // log5(5^9) = 9
    ASSERT_EQ(num, Number(9), "Logarithms");

    num = (Number(47.6) ^ 5).logBase(47.6); // log47.6(47.6^5) = 5
    ASSERT_EQ(num, Number(5), "Log-Exponential Inverse");
}

void comparisonTests() {
    std::cout << "\033[96mComparison Tests\033[0m" << std::endl;
    Number num1 = Number(758); // 758
    Number num2; // 0
    Number num3 = Number(666); // 666
    ASSERT_EQ(num2 < num1, true, "Less Than1");
    ASSERT_EQ(num2 < num3, true, "Less Than2");
    ASSERT_EQ(num2 <= num2, true, "Less Than Equal1");
    ASSERT_EQ(num2 <= num3, true, "Less Than Equal2");
    ASSERT_EQ(num1 > num2, true, "Greater Than1");
    ASSERT_EQ(num3 > num2, true, "Greater Than2");
    ASSERT_EQ(num2 >= num2, true, "Greater Than Equal1");
    ASSERT_EQ(num3 == num1, false, "Greater Than Equal2");
    ASSERT_EQ(Number(1) > 1, false, "1 > 1 = false");
}

int runTests() {
    constructorTests();
    basicMathTests();
    complexMathTests();
    comparisonTests();

    return 0;
}
