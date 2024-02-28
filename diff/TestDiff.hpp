#include "Diff.h"

void TestDiff()
{
  using namespace ADAAI::Diff;
  double x = 3, y = 1;
  auto   res = Differentiator( ExampleFunction, x, y );
  std::cout << "Derivative dx of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << "\n";
  // in reality, it is e^x * cos(e^x + 2y) ≈ −12.3968826882

  auto res2 = Differentiator<D::XY, Method::Stencil5>( ExampleFunction, x, y );
  std::cout << "Derivative dxdy of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res2 << "\n";
  // in reality, it is -2e^x * y * sin(e^x + y^2) ≈ −31.6067140403

  std::cout << "===--===---===---===--===\n\n";
}