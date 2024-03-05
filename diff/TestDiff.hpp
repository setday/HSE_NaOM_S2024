#pragma once

#include <iomanip>
#include <iostream>

#include "Diff.hpp"

using namespace ADAAI::Diff;

/// \brief result pretty printing
/// \tparam method to use
/// \param x, y - point
/// \param result - given approximation
/// \param real - real value of f(x, y)
template<Method m>
void printResult( double x, double y, double result, double real )
{
  std::cout << std::left << std::setw( 20 ) << Methods[int( m )]
            << "| x=" << std::setw( 5 ) << x
            << "| y=" << std::setw( 5 ) << y
            << "| res=" << std::setw( 20 ) << result
            << "| abs=" << std::setw( 20 ) << std::abs( result - real ) << "\n";
}

/// brief tests all mathods in point(x, y) of function f
/// \tparam D - derivative to count
/// \param f - function (usually double) to calculate derivative for
/// \param F - function f FwdAAD analogue for FwdAAD method
/// \param x, y - point
/// \param real - real value of the function derivative
template<D d, typename Callable, typename Callable2>
void TestCase( Callable f = ExampleFunction, Callable2 F = AAD::ExampleFunctionAAD, double x = 0.0, double y = 0.0, double real = 0.0 )
{
  std::cout << "\n=========================\n";
  std::cout << "Derivative counted: " << DTypes[int( d )] << "\n";
  auto res = Differentiator<Method::Stencil3, d>( f, x, y );
  printResult<Method::Stencil3>( x, y, res, real );
  res = Differentiator<Method::Stencil3Extra, d>( f, x, y );
  printResult<Method::Stencil3Extra>( x, y, res, real );
  res = Differentiator<Method::Stencil5, d>( f, x, y );
  printResult<Method::Stencil5>( x, y, res, real );
  res = Differentiator<Method::Stencil5Extra, d>( f, x, y ); // better!
  printResult<Method::Stencil5Extra>( x, y, res, real );

  res = Differentiator<d>( F, AAD::FwdAAD::X( x ), AAD::FwdAAD::Y( y ) );
  printResult<Method::FwdAAD>( x, y, res, real );
  std::cout << "true value ≈ " << real << "\n=========================\n";
}

/// \brief tests functions ExampleFunction and ExampleFunction2 for some derivatives
void TestDiff()
{
  std::cout << std::setprecision( 15 );

  double x = 3, y = 1;

  double real = -12.3968826882;
  TestCase<D::X>( ExampleFunction, AAD::ExampleFunctionAAD, x, y, real );

  real = -31.6067140403;
  TestCase<D::XY>( ExampleFunction, AAD::ExampleFunctionAAD, x, y, real );

  x = 45.2, y = 4653;
  real = 44.9318047384;
  TestCase<D::Y>( ExampleFunction2, AAD::ExampleFunctionAAD2, x, y, real );

  real = -222.269146379;
  TestCase<D::YY>( ExampleFunction2, AAD::ExampleFunctionAAD2, x, y, real );

  real = -22879.9426109;
  TestCase<D::XY>( ExampleFunction2, AAD::ExampleFunctionAAD2, x, y, real );

  std::cout << "\n===--===---===---===--===\n\n";
}
