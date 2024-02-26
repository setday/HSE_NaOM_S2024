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
  //  exp_range_tests<Method::Fourier>();  /// TODO: Make an interface

  exp_range_tests<Method::Fourier>();

  return 0;
}