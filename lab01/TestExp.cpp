#include <cassert>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

void fix_precision( int n )
{ // fixes the precision of output
  std::cout.precision( n );
  std::cout << std::fixed;
}

bool check_exp()
{
  long double test_set[] = {
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
  };

  std::cout << "=? Testing exp(x) for test set (1.0, 1345442354523432.0, -1345442354523432.0, 0.0, -0.0, 10.1, -10.1, -100.0, 0.0000001, -0.0000001, -656.0)\n";

  for ( auto x : test_set )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( x ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( x ) ) );
  }

  std::cout << "=> Test passed for test set passed!\n";

  float border = 90.0;
  float step   = 0.005;
  float value  = -border;

  std::cout << "=? Testing exp(x) for x in interval [" << -border << ", " << border << "] with step " << step << "\n";

  while ( value <= border )
  {
    assert( ( test<float, ADAAI::Exp, std::exp>( value ) ) );
    assert( ( test<double, ADAAI::Exp, std::exp>( value ) ) );
    assert( ( test<long double, ADAAI::Exp, std::exp>( value ) ) );

    value += step;
  }

  std::cout << "=> Test passed for interval [" << -border << ", " << border << "] with step " << step << " passed!\n";

  std::cout << "===>>> All test passed!\n";

  return true;
}

int main()
{
  fix_precision( 10 );
  assert( check_exp() );
  return 0;
}
