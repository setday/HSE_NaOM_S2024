#pragma once

#include <cmath>

#include "../lab01/Consts.hpp"

template<typename T, T MimicFunction( T ), T RealFunction( T )>
bool test( T x )
{ // calculates relative error
  T got      = MimicFunction( x );
  T expected = RealFunction( x );

  if ( std::isnan( got ) )
  {
    return std::isnan( expected );
  }

  if ( std::isinf( got ) )
  {
    return std::isinf( expected );
  }

  T delta = 150 * ADAAI::CONST::DELTA<T>;

  if ( x <= 0 )
  {
    return std::abs( got - expected ) < delta;
  }

  return std::abs( got / expected - 1.0 ) < delta;
}
