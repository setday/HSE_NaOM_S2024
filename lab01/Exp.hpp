#pragma once

#include <cmath>
#include <iostream>
#include <type_traits>

#include "Consts.hpp"

namespace ADAAI
{
  namespace core
  {
    template<typename F>
      requires std::is_floating_point_v<F>
    constexpr F Exp_( F x )
    { // computes exp(x) using Taylor series
      F   f1 = 0, term = 1;
      int n = 0;

      while ( true )
      {
        if ( ( x > 0 && std::abs( term ) * CONST::SQRT2<F> < CONST::DELTA<F> ) || ( x <= 0 && std::abs( term ) < CONST::DELTA<F> ) )
        {
          break;
        }

        f1 += term;
        n++;
        term *= x / n;
      }

      return f1;
    }
  } // namespace core

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
    if ( frac_part > 0.5 )
    {
      frac_part--;
      n++;
    }
    F x2 = CONST::LN2<F> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    F E2 = core::Exp_( x2 );
    return ldexp( E2, n );
  }
} // namespace ADAAI
