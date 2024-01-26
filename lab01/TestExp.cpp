#include "Exp.hpp"
#include <cmath>
#include <iostream>

void test_float(float x) {
    float EXP = ADAAI::Exp(x);
    float exp = std::exp(x);
    std::cout << EXP << ' ' << exp << ' ' << std::abs(EXP - exp) << '\n';
}

void test_double(double x) {
    double EXP = ADAAI::Exp(x);
    double exp = std::exp(x);
    std::cout << EXP << ' ' << exp << ' ' << std::abs(EXP - exp) << '\n';
}

void test_ldouble(long double x) {
    long double EXP = ADAAI::Exp(x);
    long double exp = std::exp(x);
    std::cout << EXP << ' ' << exp << ' ' << std::abs(EXP - exp) << '\n';
}

int main() {
    test_float(1.0);
    test_double(1.0);
    test_ldouble(1.5);
    // std::cout << ADAAI::Exp(x4) << '\n'; // this will fail
    return 0;
}
