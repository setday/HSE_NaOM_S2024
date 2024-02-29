#include "Diff.h"

void TestDiff()
{
  std::cout << std::setprecision( 15 );

  using namespace ADAAI::Diff;
  double x = 3, y = 1;
  double real = -12.3968826882;
  auto   res  = Differentiator<Method::Stencil3>( ExampleFunction, x, y );
  std::cout << "Derivative dx using Stencil3 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil3Extra>( ExampleFunction, x, y );
  std::cout << "Derivative dx using extrapolation on top of Stencil3 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5>( ExampleFunction, x, y );
  std::cout << "Derivative dx using Stencil5 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5Extra, D::X>( ExampleFunction, x, y ); // better!
  std::cout << "Derivative dx using extrapolation on top of Stencil5 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  std::cout << "in reality, it is e^x * cos(e^x + y^2) ≈ " << real;

  std::cout << "\n\n";

  real = -31.6067140403;
  res  = Differentiator<Method::Stencil3, D::XY>( ExampleFunction, x, y );
  std::cout << "Derivative dxdy using 3-points of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil3Extra, D::XY>( ExampleFunction, x, y );
  std::cout << "Derivative dxdy using extrapolation on top of Stencil3 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5, D::XY>( ExampleFunction, x, y );
  std::cout << "Derivative dxdy using 5-points of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5Extra, D::XY>( ExampleFunction, x, y ); // better!
  std::cout << "Derivative dxdy using extrapolation on top of Stencil5 of sin( e^x + y^2 ) at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  std::cout << "in reality, it is -2e^x * y * sin(e^x + y^2) ≈ −31.6067140403";
  std::cout << "\n===--===---===---===--===\n\n";
}
