#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "Exp.hpp"

void fix_precision( int n )
{ // fixes the precision of output
  std::cout.precision( n );
  std::cout << std::fixed;
}

template<typename T, T MimicFunction( T ) = ADAAI::Exp, T RealFunction( T ) = std::exp>
bool test( T x )
{ // calculates relative error
  T got      = MimicFunction( x );
  T expected = RealFunction( x );
  T diff     = std::abs( got - expected ) / expected;

  //  std::cout << diff  << " = |" << expected << " - " << got << "| (diff) = |(expected) - (got)| / (expected)\n";

  if ( std::isnan( got ) )
  {
    return std::isnan( expected );
  }

  if ( std::isinf( got ) )
  {
    return std::isinf( expected );
  }

  return diff < 0.00001; // relative error is less than 0.001%
}

bool check_exp()
{
  int   border = 90;
  float step   = 0.005;
  std::cout << "Testing exp(x) for x in interval [" << -border << ", " << border << "] with step " << step << "\n";
  float f = -border;
  while ( f <= border )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( f ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( f ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( f ) ) );
    f += step;
  }
  std::cout << "All test passed\n";
  return true;
}

int main()
{
  //  fix_precision( 10 );
  assert( check_exp() );
  return 0;
}
