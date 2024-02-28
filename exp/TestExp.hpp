#include "test/TestCases.cpp"

void TestExp()
{
  using namespace ADAAI::Exp;

  exp_range_tests<Method::Taylor>(); // Estimated time: 1s
  //  exp_range_tests<Method::Pade>();   // Estimated time: 1s
  //    exp_range_tests<Method::Chebyshev>(); // Estimated time: 27s
  //    exp_range_tests<Method::ChebyshevUnused>(); // Estimated time: 34s
  //  test_case<Method::Fourier>( 0, M_PI, 0.001 ); // we implemented Fourier only for [0, pi]
  //  test_case<Method::FourierUnused>( 0, M_PI, 0.001 );
}