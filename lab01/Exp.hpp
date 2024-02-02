#pragma once

#include <cmath>
#include <climits>
#include <iostream>
#include <vector>
#include <type_traits>

#include "Consts.hpp"

namespace ADAAI
{
  /// \brief Namespace for core functions
  /// \details Contains functions for computing core functions
  namespace core
  {
    /// \brief Computes exp(x) using Taylor series
    /// \example \code Exp_( 0.1 ); \endcode
    /// \tparam F - Floating point type
    /// \param x - Value to compute
    /// \return e^x
    template<typename F>
      requires std::is_floating_point_v<F>
    constexpr F Exp_( F x )
    {
      F   result = 0;
      F   term   = 1;
      int n      = 0;

      std::vector<F> terms;
      while ( true )
      {
        if ( ( x > 0 && term * CONST::SQRT2<F> < CONST::DELTA<F> ) || ( x <= 0 && std::abs( term ) < CONST::DELTA<F> ) )
        {
          break;
        }

        terms.push_back(term);
        n++;
        term *= x / n;
      }

      for(std::size_t i = 0; i < terms.size(); ++i) {
        result += terms[terms.size() - 1 - i];
      }

      return result;
    }
  } // namespace core

  /// \brief Computes exp(x)
  /// \example \code Exp( 0.1 ); \endcode
  /// \tparam F - Floating point type
  /// \param x - Value to compute
  /// \return e^x
  template<typename F>
    requires std::is_floating_point_v<F>
  constexpr F Exp( F x )
  {
    if ( std::isnan( x ) )
    {
      return std::numeric_limits<F>::quiet_NaN();
    }

    F y = CONST::LOG2E<F> * x, int_part = 0;
    F frac_part = std::modf( y, &int_part );

    if ( int_part < INT_MIN )
    {
      return 0.0;
    }
    if ( int_part > INT_MAX )
    {
      return std::numeric_limits<F>::infinity();
    }

    int n = int( int_part ); // generic case now: e^x = 2^n * e^x2

    // if ( frac_part > 0.5 )
    // {
    //   frac_part--;
    //   n++;
    // }

    F x2 = CONST::LN2<F> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    F E2 = core::Exp_( x2 );
    F En = std::ldexp( 1.0, n ); // ldexp works very bad with non 1.0 multiplier
    F E  = En * E2;         // so we multiply it separately :)
    return E;
  }
} // namespace ADAAI
