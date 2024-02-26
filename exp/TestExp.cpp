#include "test/TestCases.cpp"

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  using namespace ADAAI::Exp;

  //  exp_range_tests<Method::Taylor>();                // Estimated time: 1s
  //  exp_range_tests<Method::Pade>();                  // Estimated time: 1s
  //  exp_range_tests<Method::Chebyshev>();             // Estimated time: 27s
  //  exp_range_tests<Method::ChebyshevExperimental>(); // Estimated time: 34s
  //  exp_range_tests<Method::Fourier>(); // we implemented Fourier only for [0, pi]

  test_case<Method::Fourier>(0, M_PI, 0.001);

  return 0;
}