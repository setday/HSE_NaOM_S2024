#pragma once

#include "Diff.hpp"

using namespace ADAAI::Diff;

template<D d, typename Callable>
void TestCase( Callable f = ExampleFunction, double x = 0.0, double y = 0.0, double real = 0.0 )
{
  auto res = Differentiator<Method::Stencil3, d>( f, x, y );
  std::cout << "Derivative using Stencil3 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil3Extra, d>( f, x, y );
  std::cout << "Derivative using extrapolation on top of Stencil3 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5, d>( f, x, y );
  std::cout << "Derivative using Stencil5 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5Extra, d>( f, x, y ); // better!
  std::cout << "Derivative using extrapolation on top of Stencil5 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";

  // can't implicitly cast f anyway
  res = Differentiator<d>( FwdAAD::ExampleFunctionAAD, FwdAAD::AAD::X( x ), FwdAAD::AAD::Y( y ) );
  std::cout << "Derivative using ADD at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  std::cout << "in reality, it is ≈ " << real;

  std::cout << "\n\n";
}

void TestDiff()
{
  std::cout << std::setprecision( 15 );
  double x = 3, y = 1;
  double real = -12.3968826882;
  TestCase<D::X>( ExampleFunction, x, y, real );
  real = -31.6067140403;
  TestCase<D::XY>( ExampleFunction, x, y, real );
  std::cout << "\n===--===---===---===--===\n\n";
}
