#include <cassert>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

using namespace ADAAI::Utils;

bool check_exp()
{
  auto triple_check = []( auto x )
  {
    return adaptive_compare<float, ADAAI::Exp, std::exp>( x ) and
           adaptive_compare<double, ADAAI::Exp, std::exp>( x ) and
           adaptive_compare<long double, ADAAI::Exp, std::exp>( x );
  };

  int global_fails = 0;
  int local_tests  = 0;
  int local_fails  = 0;
  int local_passes = 0;

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

  std::cout << "=== Test case 1 ===\n=> Testing exp(x) for adaptive_compare set (1.0, 1345442354523432.0, -1345442354523432.0, 0.0, -0.0, 10.1, -10.1, -100.0, 0.0000001, -0.0000001, -656.0)\n";

  for ( auto x : test_set )
  {
    local_tests++;

    if ( !triple_check( x ) )
    {
      //      std::cout << "=! Test value " << x << " FAILED!\n";

      local_fails++;
    }
    else
    {
      local_passes++;
    }
  }

  if ( local_fails > 0 )
  {
    std::cout << "=== Test case 1 FAILED! ===\n= " << local_passes << " / " << local_tests << " tests passed! =\n\n";
    global_fails++;
    local_fails = 0;
  }
  else
  {
    std::cout << "=== Test case 1 PASSED! ===\n\n";
  }

  float border = 100.0;
  float step   = 0.005;
  float value  = -border;

  std::cout << "=== Test case 2 ===\n=> Testing exp(x) for x in interval [" << -border << ", " << border << "] with step " << step << "\n";

  while ( value <= border )
  {
    local_tests++;

    if ( !triple_check( value ) )
    {
      //      std::cout << "=! Test value " << value << " FAILED!\n";

      local_fails++;
    }
    else
    {
      local_passes++;
    }

    value += step;
  }

  if ( local_fails > 0 )
  {
    std::cout << "=== Test case 2 FAILED! ===\n= " << local_passes << " / " << local_tests << " tests passed! =\n\n";
    global_fails++;
    local_fails = 0;
  }
  else
  {
    std::cout << "=== Test case 2 PASSED! ===\n\n";
  }

  long double special_set[] = {
      std::numeric_limits<long double>::infinity(),
      -std::numeric_limits<long double>::infinity(),
      std::numeric_limits<long double>::quiet_NaN(),
      std::numeric_limits<long double>::signaling_NaN(),
      std::numeric_limits<long double>::denorm_min(),
      std::numeric_limits<long double>::min(),
      std::numeric_limits<long double>::max(),
      std::numeric_limits<long double>::lowest(),
  };

  std::cout << "=== Test case 3 ===\n=> Testing exp(x) for special set (inf, -inf, quiet_NaN, signaling_NaN, denorm_min, min, max, lowest)\n";

  for ( auto x : special_set )
  {
    local_tests++;

    if ( !triple_check( x ) )
    {
      //      std::cout << "=! Test value " << x << " FAILED!\n";

      local_fails++;
    }
    else
    {
      local_passes++;
    }
  }

  if ( local_fails > 0 )
  {
    std::cout << "=== Test case 3 FAILED! ===\n= " << local_passes << " / " << local_tests << " tests passed! =\n\n";
    global_fails++;
    local_fails = 0;
  }
  else
  {
    std::cout << "=== Test case 3 PASSED! ===\n\n";
  }

  if ( global_fails > 0 )
  {
    std::cout << "===>>> Test FAILED! ===\n= " << global_fails << " / 3 test cases failed! =\n\n";

    return false;
  }

  std::cout << "===>>> All tests passed!\n";

  return true;
}

template<typename F>
std::size_t test_case( F left, F right, F step, int num )
{
  static auto triple_check = []( auto x )
  {
    return adaptive_compare<float, ADAAI::Exp, std::exp>( x ) &&
           adaptive_compare<double, ADAAI::Exp, std::exp>( x ) &&
           adaptive_compare<long double, ADAAI::Exp, std::exp>( x );
  };

  std::size_t tests = 0;
  F           value = left;
  while ( value < right )
  {
    ++tests;
    if ( !triple_check( value ) )
    {
      std::cout << "Test case #" << num << ". Failed for: " << value << '\n';
      std::cout << std::fixed << std::exp( value ) << '\n';
      std::cout << std::fixed << ADAAI::Exp( value ) << '\n';
      assert( false );
    }
    value += step;
  }
  return tests;
}

bool normal_tests()
{
  std::size_t tests   = 0;
  long double max_exp = 709.7827125;

  tests += test_case<long double>( -1'000'000'000, 0, 1000, 1 );
  tests += test_case<long double>( -1'000'000, 0, 1, 2 );
  tests += test_case<long double>( -1'000, 0, 0.001, 3 );
  tests += test_case<long double>( -1, 0, 0.000001, 4 );
  tests += test_case<long double>( -0.001, 0, 0.000000001, 5 );
  tests += test_case<long double>( -1.001, -1, 0.000000001, 6 );
  tests += test_case<long double>( -1'000.001, -1'000, 0.000000001, 7 );
  tests += test_case<long double>( -1'000'000.001, -1'000'000, 0.000000001, 8 );
  tests += test_case<long double>( -1'000'000'000.001, -1'000'000'000, 0.000000001, 9 );
  tests += test_case<long double>( 0, 1, 0.000001, 10 );
  // passed for 12 * eps

  tests += test_case<long double>( 0, max_exp, 0.001, 11 );
  tests += test_case<long double>( max_exp - 1, max_exp, 0.000001, 12 );
  // passed for 304 * eps

  std::cout << "Success on: " << tests << " tests\n";
  return true;
}

int main()
{
  assert( normal_tests() );
  // assert( check_exp() );
  return 0;
}
