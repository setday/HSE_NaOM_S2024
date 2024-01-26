#pragma once
#include "Consts.hpp"
#include <cmath>
#include <iostream>
#include <type_traits>

namespace ADAAI {
template <typename F>
    requires std::is_floating_point_v<F>
constexpr F Exp(F x) {
    F y = x * CONST::LOG2E<F>;
    F y1;
    F y2 =
        std::modf(y, &y1);  // here y1 is an integer part of y, y2 is fractional
    std::cout << "modf test: " << y << ' ' << y1 << ' ' << y2 << '\n';
    if (y2 > 0.5) {
        y2--;
        y1++;
    }
    if (y1 < INT_MIN) {
        return 0.0;
    }
    if (y1 > INT_MAX) {
        return 1 / 0.0;  // returns float-type infty
    }
    int n = int(y1);  // generic case now: e^x = 2^n * e^x2
    F x2 = y2 * CONST::LN2<F>;
    F E1 = core::Exp_(x2);
    return ldexp(E1, n);
}

namespace core {
template <typename F>
    requires std::is_floating_point_v<F>
constexpr F Exp_(F x) {  // let's finally compute exp using Taylor series
    F f1 = 0;
    while (true) {
        // the only thing left is to compute this damn exponent
        break;
    }
    return f1;
}
}  // namespace core
}  // namespace ADAAI
