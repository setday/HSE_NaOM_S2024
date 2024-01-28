#include <cmath>

template<typename T, T MimicFunction( T ), T RealFunction( T )>
bool test( T x )
{ // calculates relative error
  T got      = MimicFunction( x );
  T expected = RealFunction( x );
  T diff     = std::abs( got - expected );

  //  std::cout << diff  << " = |" << expected << " - " << got << "| (diff) = |(expected) - (got)| / (expected)\n";

  if ( std::isnan( got ) )
  {
    return std::isnan( expected );
  }

  if ( std::isinf( got ) )
  {
    return std::isinf( expected );
  }

  if ( expected == 0.0 )
  {
    return diff < 0.00001;
  }

  return diff / expected < 0.00001; // relative error is less than 0.001%
}
