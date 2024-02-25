#include <cassert>
#include <climits>
#include <iostream>

#include "test/TestCases.cpp"

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  exp_range_tests<ADAAI::Exp::Method::ChebyshevExperimental>(); // Estimated time: 41s
  // auto res = range_check<double, ExpSingleCheckObject<ADAAI::Exp::Method::ChebyshevExperimental, double>>( -300, 1000, 0.001 );
  // std::cout << res;
  return 0;
}
