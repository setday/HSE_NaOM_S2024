#pragma once

#include <cmath>
#include <stdexcept>

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

  enum class D : int
  {
    X,
    Y,
    XX,
    YY,
    XY,
  };

  /// \brief Example function for Differentiator in use demonstration
  double ExampleFunction( double x, double y )
  {
    return std::sin( std::exp( x ) + std::pow( y, 2 ) );
  }

  namespace FwdAAD
  {
    AAD ExampleFunctionAAD( AAD X, AAD Y )
    {
      return sin( exp( X ) + Y * Y );
    }

    template<D d = D::X, typename Callable>
    double Differentiator( Callable F = ExampleFunctionAAD, AAD X = {}, AAD Y = {} )
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
  } // namespace FwdAAD

  template<D d, typename Callable>
  double Stencil3( Callable const& f, double x, double y, double h_x = -1, double h_y = -1 )
  {
    if ( h_x <= 0 )
      h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    if ( h_y <= 0 )
      h_y = CONST::h * std::max( std::abs( y ), 1.0 );
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

  /// \param n - should be 2 at least
  template<D d, typename Callable>
  double Stencil3Extra( Callable const& f, double x, double y, int n = 2 )
  {
    double h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    double h_y = CONST::h * std::max( std::abs( y ), 1.0 );
    switch ( d )
    {
      case D::X:
        return ( n * n * Stencil3<d>( f, x, y, h_x = h_x / n ) - Stencil3<d>( f, x, y, h_x = h_x ) ) / ( n * n - 1 );
      case D::Y:
        return ( n * n * Stencil3<d>( f, x, y, h_y = h_y / n ) - Stencil3<d>( f, x, y, h_y = h_y ) ) / ( n * n - 1 );
      case D::XX:
        return ( n * n * Stencil3<d>( f, x, y, h_x = h_x / n ) - Stencil3<d>( f, x, y, h_x = h_x ) ) / ( n * n - 1 );
      case D::YY:
        return ( n * n * Stencil3<d>( f, x, y, h_y = h_y / n ) - Stencil3<d>( f, x, y, h_y = h_y ) ) / ( n * n - 1 );
      case D::XY: /// TODO: understand what should be here
        return 0;
    }
  }

  template<D d, typename Callable>
  double Stencil5( Callable const& f, double x, double y, double h_x = -1, double h_y = -1 )
  {
    if ( h_x <= 0 )
      h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    if ( h_y <= 0 )
      h_y = CONST::h * std::max( std::abs( y ), 1.0 );
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
      { // pls, don't ask:)
        return ( -f( x + 2 * h_x, y + 2 * h_y ) + 16 * f( x + h_x, y + h_y ) +
                 f( x + 2 * h_x, y - 2 * h_y ) - 16 * f( x + h_x, y - h_y ) +
                 f( x - 2 * h_x, y + 2 * h_y ) - 16 * f( x - h_x, y + h_y ) +
                 -f( x - 2 * h_x, y - 2 * h_y ) + 16 * f( x - h_x, y - h_y ) ) /
               ( 48 * h_x * h_y );
      }
    }
  }

  /// \param n - should be 2 at least
  template<D d, typename Callable>
  double Stencil5Extra( Callable const& f, double x, double y, int n = 2 )
  {
    double h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    double h_y = CONST::h * std::max( std::abs( y ), 1.0 );
    switch ( d )
    {
      case D::X:
        return ( n * n * Stencil5<d>( f, x, y, h_x = h_x / n ) - Stencil5<d>( f, x, y, h_x = h_x ) ) / ( n * n - 1 );
      case D::Y:
        return ( n * n * Stencil5<d>( f, x, y, h_y = h_y / n ) - Stencil5<d>( f, x, y, h_y = h_y ) ) / ( n * n - 1 );
      case D::XX:
        return ( n * n * Stencil5<d>( f, x, y, h_x = h_x / n ) - Stencil5<d>( f, x, y, h_x = h_x ) ) / ( n * n - 1 );
      case D::YY:
        return ( n * n * Stencil5<d>( f, x, y, h_y = h_y / n ) - Stencil5<d>( f, x, y, h_y = h_y ) ) / ( n * n - 1 );
      case D::XY: /// TODO: understand what should be here
        return 0;
    }
  }

  /// \brief Computes derivative of f(x, y) in a given point
  /// \tparam d - which derivative to compute (first and second order available)
  /// \tparam M - method to use
  /// \tparam Callable - function which derivative to approximate
  /// \return The computed coefficient a_k.
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
      case Method::FwdAAD:                                                                                     // terrible implicit passing of FwdAAD::ExampleFunctionAAD
        return FwdAAD::Differentiator( FwdAAD::ExampleFunctionAAD, FwdAAD::AAD::X( x ), FwdAAD::AAD::Y( y ) ); /// TODO: here we should pass AAD F(AAD, AAD) equivalent to double f(double, double) somehow
      default:
      {
        throw std::invalid_argument( "Invalid method for Differentiator" );
      }
    }
  }
} // namespace ADAAI::Diff