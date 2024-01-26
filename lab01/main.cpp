#include <iostream>

template<typename F> requires std::is_floating_point_v<F> // F in float, double, long double
constexpr F Exp(F x) {
    // calculates exp(x) using Taylor series
    return x;
}

int main() {
    float x1 = 1.0;
    double x2 = 1.0;
    long double x3 = 1.0;
    int x4 = 1;
    std::cout << Exp(x1) << '\n';
    std::cout << Exp(x2) << '\n';
    std::cout << Exp(x3) << '\n';
    // std::cout << Exp(x4) << '\n'; // this will fail
    return 0;
}
