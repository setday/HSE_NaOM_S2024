#include <cassert>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

using namespace ADAAI::Utils;

bool exp_triple_check( auto x )
{
  return adaptive_compare<float, ADAAI::Exp, std::exp>( x ) and
         adaptive_compare<double, ADAAI::Exp, std::exp>( x ) and
         adaptive_compare<long double, ADAAI::Exp, std::exp>( x );
}

template<typename F>
std::size_t test_case( F left, F right, F step, std::size_t num )
{
  auto result = range_check<F, exp_triple_check>( left, right, step, true );

  if ( result.passed )
  {
    return result.tests_number;
  }

  std::cout << "Test case #" << num << ". Failed for: " << result.first_fail << '\n';
  std::cout << std::fixed << std::exp( result.first_fail ) << '\n';
  std::cout << std::fixed << ADAAI::Exp( result.first_fail ) << '\n';
  assert( false );
  return -1;
}

bool exp_standard_tests()
{
  std::size_t global_fails = 0;

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

  auto result_case_1 = array_check<long double, exp_triple_check>( test_set, 11 );

  if ( result_case_1.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_1 << "\n\n";

  auto result_case_2 = range_check<float, exp_triple_check>( -100.0, 100.0, 0.005 );

  if ( result_case_2.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_2 << "\n\n";

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

  auto result_case_3 = array_check<long double, exp_triple_check>( special_set, 8 );

  if ( result_case_3.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_3 << "\n\n";

  if ( global_fails > 0 )
  {
    std::cout << "===>>> Test FAILED! ===\n= " << global_fails << " / 3 test cases failed! =\n\n";

    return false;
  }

  std::cout << "===>>> All tests passed!\n";

  return true;
}

/// \brief Tests the range of the exp function
/// \details Average test time: 18s
/// \return True if all tests passed
bool exp_range_tests()
{
  std::size_t tests   = 1;
  long double max_exp = 709.7827125;

  std::cout << range_check<long double, exp_triple_check>( -1000, 1000, 0.1, true ) << '\n';
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
  // assert( exp_range_tests() );
  // assert( exp_standard_tests() );

  long double max = 0;
  for ( long double f = -ADAAI::CONST::LN2<long double> * 50.0; f <= 0; f += ADAAI::CONST::LN2<long double> / 1e5 )
  {
    max = std::max( max, std::abs( std::exp( f ) - ADAAI::Exp<ADAAI::Method::Pade>( f ) ) );
  }
  for ( long double f = 0; f <= ADAAI::CONST::LN2<long double> * 50.0; f += ADAAI::CONST::LN2<long double> / 1e5 )
  {
    max = std::max( max, std::abs( std::exp( f ) - ADAAI::Exp<ADAAI::Method::Pade>( f ) ) / std::exp( f ) );
    max = std::max( max, std::abs( std::exp( f ) - ADAAI::Exp<ADAAI::Method::Pade>( f ) ) / ADAAI::Exp<ADAAI::Method::Pade>( f ) );
  }
  std::cout << std::setprecision( 50 ) << max / std::numeric_limits<long double>::epsilon();

  // x \in [-50*ln(2), 5*ln(2)]
  //                   *eps
  // tailor   49.5
  // [10, 10] 18.6
  // [19, 1]  18.6
  // [1, 19]  18.75
  // [0, 20]  18.63
  // [15, 5]  18.59
  // [5, 15]  18.68

  return 0;
}
