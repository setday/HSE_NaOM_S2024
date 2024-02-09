#include <cassert>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

using namespace ADAAI::Utils;

long double error = 0.0;

template<ADAAI::Method M>
bool exp_triple_check( auto x )
{
  auto float_check       = adaptive_compare<float, ADAAI::Exp<float, M>, std::exp>( x );
  auto double_check      = adaptive_compare<double, ADAAI::Exp<double, M>, std::exp>( x );
  auto long_double_check = adaptive_compare<long double, ADAAI::Exp<long double, M>, std::exp>( x );

  error = std::max( error, (long double)float_check.second );
  error = std::max( error, (long double)double_check.second );
  error = std::max( error, (long double)long_double_check.second );

  return float_check.first and double_check.first and long_double_check.first;
}

template<typename F, ADAAI::Method M = ADAAI::Method::Taylor>
std::size_t test_case( F left, F right, F step, std::size_t num )
{
  error = 0.0;
  auto result = range_check<F, exp_triple_check<M>>( left, right, step, true );
  std::cout << result << " num of eps: " << error << "\n\n";

  if ( result.passed )
  {
    return result.tests_number;
  }

//  std::cout << "Test case #" << num << ". Failed for: " << result.first_fail << '\n';
//  std::cout << std::fixed << std::exp( result.first_fail ) << '\n';
//  std::cout << std::fixed << ADAAI::Exp( result.first_fail ) << '\n';
  assert( false );
  return -1;
}

template<ADAAI::Method M = ADAAI::Method::Taylor>
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

  error = 0.0;
  auto result_case_1 = array_check<long double, exp_triple_check<M>>( test_set, 11 );

  if ( result_case_1.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_1 << " num of eps: " << error << "\n\n";

  error = 0.0;
  auto result_case_2 = range_check<float, exp_triple_check<M>>( -100.0, 100.0, 0.005 );

  if ( result_case_2.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_2 << " num of eps: " << error << "\n\n";

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

  error = 0.0;
  auto result_case_3 = array_check<long double, exp_triple_check<M>>( special_set, 8 );

  if ( result_case_3.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_3 << " num of eps: " << error << "\n\n";

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
template<ADAAI::Method M = ADAAI::Method::Taylor>
bool exp_range_tests()
{
  long double max_exp = 709.7827125;
  std::size_t tests = 0;

  error = 0.0;
  std::cout << range_check<long double, exp_triple_check<M>>( -1000, 1000, 0.1, true ) << " num of eps: " << error << "\n\n";
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

  tests += test_case<long double>( 0, max_exp, 0.001, 11 );
  tests += test_case<long double>( max_exp - 1, max_exp, 0.000001, 12 );

  return true;
}

int main()
{
  assert( exp_range_tests() );
  assert( exp_standard_tests() );
  assert( exp_range_tests<ADAAI::Method::Pade>() );
  assert( exp_standard_tests<ADAAI::Method::Pade>() );
  return 0;
}
