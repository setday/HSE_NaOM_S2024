#include "Diff.h"

void TestDiff()
{
  std::cout << std::setprecision( 10 );

  using namespace ADAAI::Diff;
  double x = 3, y = 1;
  auto   res = Differentiator( ExampleFunction, x, y ); // Differentiator<D::X, Method::Stencil5>
  std::cout << "Derivative dx of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << "\n";
  // in reality, it is e^x * cos(e^x + 2y) ≈ −12.3968826882

  auto res2 = Differentiator<D::XY, Method::Stencil3>( ExampleFunction, x, y );
  std::cout << "Derivative dxdy using 3-points of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res2 << "\n";
  auto res3 = Differentiator<D::XY, Method::Stencil5>( ExampleFunction, x, y ); // better!
  std::cout << "Derivative dxdy using 5-points of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res3 << "\n";
  // in reality, it is -2e^x * y * sin(e^x + y^2) ≈ −31.6067140403

  std::cout << "===--===---===---===--===\n\n";
}