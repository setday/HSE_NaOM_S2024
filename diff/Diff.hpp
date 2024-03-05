#pragma once

#include <cmath>
#include <stdexcept>
#include <string>

#include "../utils/Consts.hpp"
#include "methods/FwdAAD.hpp"

namespace ADAAI::Diff
{
  enum class Method : int
  {
    Stencil3,
    Stencil3Extra,
    Stencil5,
    Stencil5Extra,
    FwdAAD,
  };

  constexpr std::string_view Methods[] = {
      "Stencil3",
      "Stencil3Extra",
      "Stencil5",
      "Stencil5Extra",
      "FwdAAD",
  };

  enum class D : int
  {
    X,
    Y,
    XX,
    YY,
    XY,
  };

  constexpr std::string_view DTypes[] = {
      "X",
      "Y",
      "XX",
      "YY",
      "XY",
  };

  /// \brief First function for Differentiator in use demonstration
  double ExampleFunction( double x, double y )
  {
    return std::sin( std::exp( x ) + std::pow( y, 2 ) );
  }

  /// \brief Second function for Differentiator in use demonstration
  double ExampleFunction2( double x, double y )
  {
    return -x * x / y + std::cos( x * y );
  }

  /// \brief three points stencil method
  /// \tparam D - derivative to compute
  /// \param f - function which derivative to compute
  /// \param x, y - point
  template<D d, typename Callable>
  double Stencil3( Callable const& f, double x, double y, double h_x = CONST::h, double h_y = CONST::h )
  {
    switch ( d )
    {
      case D::X:
        return ( f( x + h_x, y ) - f( x - h_x, y ) ) / ( 2 * h_x );
      case D::Y:
        return ( f( x, y + h_y ) - f( x, y - h_y ) ) / ( 2 * h_y );
      case D::XX:
        return ( f( x + h_x, y ) + f( x - h_x, y ) - 2 * f( x, y ) ) / ( h_x * h_x );
      case D::YY:
        return ( f( x, y + h_y ) + f( x, y - h_y ) - 2 * f( x, y ) ) / ( h_y * h_y );
      case D::XY:
        return ( f( x + h_x, y + h_y ) - f( x - h_x, y + h_y ) - f( x + h_x, y - h_y ) + f( x - h_x, y - h_y ) ) / ( 4 * h_x * h_y );
    }
  }

  /// \brief three points stencil method with extrapolation
  /// \tparam D - derivative to compute
  /// \param f - function which derivative to compute
  /// \param x, y - point
  /// \param n - must be at least 2
  template<D d, typename Callable>
  double Stencil3Extra( Callable const& f, double x, double y, double h_x = CONST::h, double h_y = CONST::h, int n = 2 )
  {
    int squared = n * n;
    switch ( d )
    {
      case D::X:
        return ( squared * Stencil3<d>( f, x, y, h_x = h_x / n ) - Stencil3<d>( f, x, y, h_x = h_x ) ) / ( squared - 1 );
      case D::Y:
        return ( squared * Stencil3<d>( f, x, y, h_y = h_y / n ) - Stencil3<d>( f, x, y, h_y = h_y ) ) / ( squared - 1 );
      case D::XX:
        return ( squared * Stencil3<d>( f, x, y, h_x = h_x / n ) - Stencil3<d>( f, x, y, h_x = h_x ) ) / ( squared - 1 );
      case D::YY:
        return ( squared * Stencil3<d>( f, x, y, h_y = h_y / n ) - Stencil3<d>( f, x, y, h_y = h_y ) ) / ( squared - 1 );
      case D::XY:
        return ( squared * Stencil3<d>( f, x, y, h_x = h_x / n, h_y = h_y / n ) - Stencil3<d>( f, x, y, h_x = h_x, h_y = h_y ) ) / ( squared - 1 );
    }
  }

  /// \brief five points stencil method
  /// \tparam D - derivative to compute
  /// \param f - function which derivative to compute
  /// \param x, y - point
  template<D d, typename Callable>
  double Stencil5( Callable const& f, double x, double y, double h_x = CONST::h, double h_y = CONST::h )
  {
    switch ( d )
    {
      case D::X:
        return ( -f( x + 2 * h_x, y ) + 8 * f( x + h_x, y ) - 8 * f( x - h_x, y ) + f( x - 2 * h_x, y ) ) / ( 12 * h_x );
      case D::Y:
        return ( -f( x, y + 2 * h_y ) + 8 * f( x, y + h_y ) - 8 * f( x, y - h_y ) + f( x, y - 2 * h_y ) ) / ( 12 * h_y );
      case D::XX:
        return ( -f( x + 2 * h_x, y ) + 16 * f( x + h_x, y ) - 30 * f( x, y ) + 16 * f( x - h_x, y ) - f( x - 2 * h_x, y ) ) / ( 12 * h_x * h_x );
      case D::YY:
        return ( -f( x, y + 2 * h_y ) + 16 * f( x, y + h_y ) - 30 * f( x, y ) + 16 * f( x, y - h_y ) - f( x, y - 2 * h_y ) ) / ( 12 * h_y * h_y );
      case D::XY:
        return ( -f( x + 2 * h_x, y + 2 * h_y ) + 16 * f( x + h_x, y + h_y ) +
                 f( x + 2 * h_x, y - 2 * h_y ) - 16 * f( x + h_x, y - h_y ) +
                 f( x - 2 * h_x, y + 2 * h_y ) - 16 * f( x - h_x, y + h_y ) +
                 -f( x - 2 * h_x, y - 2 * h_y ) + 16 * f( x - h_x, y - h_y ) ) /
               ( 48 * h_x * h_y );
    }
  }

  /// \brief five points stencil method with extrapolation
  /// \tparam D - derivative to compute
  /// \param f - function which derivative to compute
  /// \param x, y - point
  /// \param n - must be at least 2
  template<D d, typename Callable>
  double Stencil5Extra( Callable const& f, double x, double y, double h_x = CONST::h, double h_y = CONST::h, int n = 2 )
  {
    int squared = n * n;
    switch ( d )
    {
      case D::X:
        return ( squared * Stencil5<d>( f, x, y, h_x = h_x / n ) - Stencil5<d>( f, x, y, h_x = h_x ) ) / ( squared - 1 );
      case D::Y:
        return ( squared * Stencil5<d>( f, x, y, h_y = h_y / n ) - Stencil5<d>( f, x, y, h_y = h_y ) ) / ( squared - 1 );
      case D::XX:
        return ( squared * Stencil5<d>( f, x, y, h_x = h_x / n ) - Stencil5<d>( f, x, y, h_x = h_x ) ) / ( squared - 1 );
      case D::YY:
        return ( squared * Stencil5<d>( f, x, y, h_y = h_y / n ) - Stencil5<d>( f, x, y, h_y = h_y ) ) / ( squared - 1 );
      case D::XY:
        return ( squared * Stencil5<d>( f, x, y, h_x = h_x / n, h_y = h_y / n ) - Stencil5<d>( f, x, y, h_x = h_x, h_y = h_y ) ) / ( squared - 1 );
    }
  }

  /// \brief Computes the derivative of f(x, y) at the given point
  /// \tparam d is an order of the derivative to compute (first and second order available)
  /// \tparam M is a method to use
  /// \param Callable is a function which derivative to approximate
  /// \return The computed derivative
  template<Method M = Method::Stencil5, D d = D::X, typename Callable>
  double Differentiator( Callable f = ExampleFunction, double x = 0, double y = 0 )
  {
    switch ( M )
    {
      case Method::Stencil3:
        return Stencil3<d>( f, x, y );
      case Method::Stencil3Extra:
        return Stencil3Extra<d>( f, x, y );
      case Method::Stencil5:
        return Stencil5<d>( f, x, y );
      case Method::Stencil5Extra:
        return Stencil5Extra<d>( f, x, y );
      default:
        throw std::invalid_argument( "Invalid method for Differentiator" );
    }
  }

  /// \brief Computes the derivative of f(x, y) at the given point using FwdAAD
  /// \tparam d is an order of the derivative to compute (first and second order available)
  /// \param Callable is an FwdAAD function which derivative to approximate
  /// \return The computed derivative
  template<D d = D::X, typename Callable>
  double Differentiator( Callable F = AAD::ExampleFunctionAAD, AAD::FwdAAD X = {}, AAD::FwdAAD Y = {} )
  {
    auto val = F( X, Y );
    switch ( d )
    {
      case D::X:
        return val.X();
      case D::Y:
        return val.Y();
      case D::XX:
        return val.XX();
      case D::YY:
        return val.YY();
      case D::XY:
        return val.XY();
    }
  }
} // namespace ADAAI::Diff
