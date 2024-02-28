#include "Diff.h"

void TestDiff()
{
  using namespace ADAAI::Diff;
  double x = 3, y = 1;
  auto   res = Differentiator( ExampleFunction, x, y );
  std::cout << "Derivative dx of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << "\n";
  // in reality, it is e^3 * cos(e^3 + 1) ≈ −12.3968826882
}