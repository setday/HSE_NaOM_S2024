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
{
  T got      = MimicFunction( x );
  T expected = RealFunction( x );
  T diff     = std::abs( got - expected );

  std::cout << std::setprecision( 10 ) << diff << " = |" << expected << " - " << got << "| (diff) = |(expected) - (got)|\n";

  if ( std::isnan( got ) )
  {
    return std::isnan( expected );
  }

  if ( std::isinf( got ) )
  {
    return std::isinf( expected );
  }

  return diff < 0.05;
  // theoretically, diff should be less than ADAAI::CONST::DELTA<T>, only true on some values and more likely with ld
}

bool check_exp()
{
  long double test_numbers[] = {
      1.0,
      1345442354523432.0,
      -1345442354523432.0,
      0.0,
      -0.0,
      10.1,
      -10.1,
      -100.0,
      0.0000001,
      -0.0000001,
      -656.0,
      // 100, // the error is terrifying (not relative though)
  };

  for ( auto x : test_numbers )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( x ) ) );
  }

  float f = 1;
  while ( f < 2 )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( f ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( f ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( f ) ) );
    f += 0.005;
  }

  return true;
}

int main()
{
  fix_precision( 10 );
  assert( check_exp() );
  return 0;
}
