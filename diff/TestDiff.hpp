#pragma once

#include "Diff.hpp"

using namespace ADAAI::Diff;

void printResult( const std::string& method, double x, double y, double result, double absolute )
{
  std::cout << std::left << std::setw( 13 ) << method << "| x=" << std::setw( 2 ) << x
            << "| y=" << std::setw( 2 ) << y
            << "| res=" << std::setw( 18 ) << result
            << "| abs=" << absolute << std::endl;
}

template<D d, typename Callable>
void TestCase( Callable f = ExampleFunction, double x = 0.0, double y = 0.0, double real = 0.0 )
{
  std::cout << "\n=========================\nTEST CASE\n";
  auto res = Differentiator<Method::Stencil3, d>( f, x, y );
  printResult( "Stencil3", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil3Extra, d>( f, x, y );
  printResult( "Stencil3Extra", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil5, d>( f, x, y );
  printResult( "Stencil5", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil5Extra, d>( f, x, y ); // better!
  printResult( "Stencil5Extra", x, y, res, std::abs( res - real ) );

  // can't implicitly cast f anyway
  res = Differentiator<d>( FwdAAD::ExampleFunctionAAD, FwdAAD::AAD::X( x ), FwdAAD::AAD::Y( y ) );
  printResult( "ADD", x, y, res, std::abs( res - real ) );
  std::cout << "true value ≈ " << real << "\n=========================\n";
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
