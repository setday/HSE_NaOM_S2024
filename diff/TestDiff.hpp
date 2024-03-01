#pragma once

#include "Diff.hpp"

using namespace ADAAI::Diff;

template<D d>
void TestCase( double x, double y, double real )
{
  auto res = Differentiator<Method::Stencil3, d>( ExampleFunction, x, y );
  std::cout << "Derivative using Stencil3 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil3Extra, d>( ExampleFunction, x, y );
  std::cout << "Derivative using extrapolation on top of Stencil3 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5, d>( ExampleFunction, x, y );
  std::cout << "Derivative using Stencil5 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::Stencil5Extra, d>( ExampleFunction, x, y ); // better!
  std::cout << "Derivative using extrapolation on top of Stencil5 at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  res = Differentiator<Method::FwdAAD, d>( ExampleFunction, x, y );
  std::cout << "Derivative using ADD at point " << x << " " << y << " is " << res << " abs: " << std::abs( res - real ) << "\n";
  std::cout << "in reality, it is ≈ " << real;

  std::cout << "\n\n";
}

void TestDiff()
{
  std::cout << std::setprecision( 15 );
  double x = 3, y = 1;
  double real = -12.3968826882;
  TestCase<D::X>( x, y, real );
  real = -31.6067140403;
  TestCase<D::XY>( x, y, real );
  std::cout << "\n===--===---===---===--===\n\n";
}
