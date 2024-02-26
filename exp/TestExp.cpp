#include <cassert>
#include <climits>
#include <iostream>

#include "../utils/Clock.hpp"
#include "test/TestCases.cpp"

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  exp_range_tests<ADAAI::Exp::Method::Taylor>();                // Estimated time: 1s
  exp_range_tests<ADAAI::Exp::Method::Pade>();                  // Estimated time: 1s
  exp_range_tests<ADAAI::Exp::Method::Chebyshev>();             // Estimated time: 27s
  exp_range_tests<ADAAI::Exp::Method::ChebyshevExperimental>(); // Estimated time: 34s

  return 0;
}