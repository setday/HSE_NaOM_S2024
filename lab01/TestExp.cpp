#include <cassert>
#include <cmath>
#include <iostream>

#include "Exp.hpp"

template<typename T, T MimicFunction( T ) = ADAAI::Exp, T RealFunction( T ) = std::exp>
bool test( T x )
{
  T got      = MimicFunction( x );
  T expected = RealFunction( x );
  T diff     = std::abs( got - expected );

  std::cout << diff << " = |" << expected << " - " << got << "| (diff) = |(expected) - (got)|\n";

  if ( std::isnan( got ) )
  {
    return std::isnan( expected );
  }

  if ( std::isinf( got ) )
  {
    return std::isinf( expected );
  }

  return diff < 1e-6;
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
      100.0,
      -100.0,
      0.0000001,
      -0.0000001,
      656.0,
      -656.0,
  };

  for ( auto x : test_numbers )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( x ) ) );
  }

  return true;
}

int main()
{
  assert( check_exp() );

  // std::cout << ADAAI::Exp(x4) << '\n'; // this will fail
  return 0;
}
