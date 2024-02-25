#include <cassert>
#include <climits>
#include <iostream>

#include "test/TestCases.cpp"

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  //  exp_range_tests<ADAAI::Exp::Method::Taylor>(); // Estimated time: 41s
  auto res = range_check<double, ExpSingleCheckObject<ADAAI::Exp::Method::ChebyshevExperimental, double>>( -300, 1000, 0.001 );
  std::cout << res;
  auto res2 = range_check<double, ExpSingleCheckObject<ADAAI::Exp::Method::Chebyshev, double>>( -300, 1000, 0.001 );
  std::cout << res2;
  return 0;
}
