#pragma once

#include <iomanip>
#include <iostream>

#include "Diff.hpp"

using namespace ADAAI::Diff;

void printResult( const std::string& method, double x, double y, double result, double absolute )
{
  std::cout << std::left << std::setw( 20 ) << method
            << "| x=" << std::setw( 5 ) << x
            << "| y=" << std::setw( 5 ) << y
            << "| res=" << std::setw( 20 ) << result
            << "| abs=" << std::setw( 20 ) << absolute << "\n";
}

template<D d, typename Callable, typename Callable2>
void TestCase( Callable f = ExampleFunction, Callable2 F = FwdAAD::ExampleFunctionAAD, double x = 0.0, double y = 0.0, double real = 0.0 )
{
  std::cout << "\n=========================\nTEST CASE\n";
  std::cout << "Derivative counted: " << DTypes[int( d )] << "\n";
  auto res = Differentiator<Method::Stencil3, d>( f, x, y );
  printResult( "Stencil3", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil3Extra, d>( f, x, y );
  printResult( "Stencil3Extra", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil5, d>( f, x, y );
  printResult( "Stencil5", x, y, res, std::abs( res - real ) );
  res = Differentiator<Method::Stencil5Extra, d>( f, x, y ); // better!
  printResult( "Stencil5Extra", x, y, res, std::abs( res - real ) );

  res = Differentiator<d>( F, FwdAAD::AAD::X( x ), FwdAAD::AAD::Y( y ) );
  printResult( "ADD", x, y, res, std::abs( res - real ) );
  std::cout << "true value ≈ " << real << "\n=========================\n";
}

void TestDiff()
{
  std::cout << std::setprecision( 15 );

  double x = 3, y = 1;

  double real = -12.3968826882;
  TestCase<D::X>( ExampleFunction, FwdAAD::ExampleFunctionAAD, x, y, real );

  real = -31.6067140403;
  TestCase<D::XY>( ExampleFunction, FwdAAD::ExampleFunctionAAD, x, y, real );

  x = 45.2, y = 4653;
  real = 44.9318047384;
  TestCase<D::Y>( ExampleFunction2, FwdAAD::ExampleFunctionAAD2, x, y, real );

  real = -222.269146379;
  TestCase<D::YY>( ExampleFunction2, FwdAAD::ExampleFunctionAAD2, x, y, real );

  real = -22879.9426109;
  TestCase<D::XY>( ExampleFunction2, FwdAAD::ExampleFunctionAAD2, x, y, real );

  std::cout << "\n===--===---===---===--===\n\n";
}
