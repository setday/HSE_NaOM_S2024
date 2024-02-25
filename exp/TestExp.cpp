#include <cassert>
#include <climits>
#include <iostream>

#include "test/TestCases.cpp"
#include "../utils/Clock.hpp"

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  exp_range_tests<ADAAI::Exp::Method::Taylor>(); // Estimated time: 27s
  exp_range_tests<ADAAI::Exp::Method::Pade>(); // Estimated time: 27s
  exp_range_tests<ADAAI::Exp::Method::Chebyshev>(); // Estimated time: 27s
  exp_range_tests<ADAAI::Exp::Method::ChebyshevExperimental>(); // Estimated time: 34s

  return 0;
}
