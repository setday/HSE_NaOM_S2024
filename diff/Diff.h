#pragma once

#include <cmath>
#include <stdexcept>

#include "../utils/Consts.hpp"

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
    XY
  };

  template<D d, Method M, typename Callable>
  double Stencil3( Callable const& f, double x, double y )
  {
    double h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    double h_y = CONST::h * std::max( std::abs( y ), 1.0 );
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
        return 0; // TODO: idk what should be here
    }
  }

  template<D d, Method M, typename Callable>
  double Stencil3Extra( Callable const& f, double x, double y )
  { /// TODO
    return 0;
  }

  template<D d, Method M, typename Callable>
  double Stencil5( Callable const& f, double x, double y )
  {
    double h_x = CONST::h * std::max( std::abs( x ), 1.0 );
    double h_y = CONST::h * std::max( std::abs( y ), 1.0 );
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
        return ( f( x + h_x, y + h_y ) - f( x - h_x, y + h_y ) - f( x + h_x, y - h_y ) + f( x - h_x, y - h_y ) ) / ( 4 * h_x * h_y );
    }
  }

  template<D d, Method M, typename Callable>
  double Stencil5Extra( Callable const& f, double x, double y )
  {
    /// TODO
    return 0;
  }

  /// \brief Example function for Differentiator in use demonstration
  double ExampleFunction( double x, double y )
  {
    return std::sin( std::exp( x ) + std::pow( y, 2 ) );
  }

  /// \brief Computes derivative of f(x, y) in a given point
  /// \tparam d - which derivative to compute (first and second order available)
  /// \tparam M - method to use
  /// \tparam Callable - function which derivative to approximate
  /// \return The computed coefficient a_k.
  template<D d = D::X, Method M = Method::Stencil5, typename Callable>
  double Differentiator( Callable f = ExampleFunction, double x = 0, double y = 0 )
  {
    switch ( M )
    {
      case Method::Stencil3:
        return Stencil3<d, M>( f, x, y );
      case Method::Stencil3Extra:
        return Stencil3Extra<d, M>( f, x, y );
      case Method::Stencil5:
        return Stencil5<d, M>( f, x, y );
      case Method::Stencil5Extra:
        return Stencil5Extra<d, M>( f, x, y );
      case Method::FwdAAD:
        return 0; /// TODO
      default:
      {
        throw std::invalid_argument( "Invalid method for Differentiator" );
      }
    }
  }
} // namespace ADAAI::Diff