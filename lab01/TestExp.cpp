#include <cassert>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

using namespace ADAAI::Utils;

template<typename T>
struct Error
{
  T error = 0.0;

  void reset()
  {
    error = 0.0;
  }

  void update( T inp )
  {
    if ( !std::isinf( inp ) )
      error = std::max( error, inp );
  }
};

template<typename T>
std::ostream& operator<<( std::ostream& os, const Error<T>& error )
{
  os << "error " << error.error << " ";
  return os;
}

Error<float>       f_error;
Error<double>      d_error;
Error<long double> ld_error;

void triple_reset()
{
  f_error.reset();
  d_error.reset();
  ld_error.reset();
}

void triple_update( float err1, double err2, long double err3 )
{
  f_error.update( err1 );
  d_error.update( err2 );
  ld_error.update( err3 );
}

void triple_error_cout()
{
  std::cout << "float " << f_error << "double " << d_error << "long double " << ld_error << "\n\n";
}

template<ADAAI::Method M>
bool exp_triple_check( auto x )
{
  auto float_check       = adaptive_compare<float, ADAAI::Exp<float, M>, std::exp>( x );
  auto double_check      = adaptive_compare<double, ADAAI::Exp<double, M>, std::exp>( x );
  auto long_double_check = adaptive_compare<long double, ADAAI::Exp<long double, M>, std::exp>( x );

  triple_update( float_check.error, double_check.error, long_double_check.error );

  return float_check.passed and double_check.passed and long_double_check.passed;
}

template<typename F, ADAAI::Method M = ADAAI::Method::Taylor>
std::size_t test_case( F left, F right, F step, std::size_t num )
{
  triple_reset();
  auto result = range_check<F, exp_triple_check<M>>( left, right, step );
  std::cout << result;
  triple_error_cout();

  if ( result.passed )
  {
    return result.tests_number;
  }

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

  triple_reset();
  auto result_case_1             = array_check<long double, exp_triple_check<M>>( test_set, 11 );
  result_case_1.test_case_number = 1;

  if ( result_case_1.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_1 << "\n\n";
  triple_error_cout();

  triple_reset();
  auto result_case_2             = range_check<float, exp_triple_check<M>>( -100.0, 100.0, 0.005 );
  result_case_2.test_case_number = 2;

  if ( result_case_2.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_2 << "\n\n";
  triple_error_cout();

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

  triple_reset();
  auto result_case_3             = array_check<long double, exp_triple_check<M>>( special_set, 8 );
  result_case_3.test_case_number = 3;

  if ( result_case_3.fails_count > 0 )
  {
    global_fails++;
  }

  std::cout << result_case_3 << "\n\n";
  triple_error_cout();

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
  if ( M == ADAAI::Method::Taylor )
  {
    std::cout << "Method used Taylor\n\n";
  }
  else
  {
    std::cout << "Method used Pade\n\n";
  }

  //  long double max_exp = 11356.6;
  std::size_t tests  = 0;
  float       border = 1000.0;

  tests += test_case<long double, M>( -border, border, 0.001, 0 );
  //  tests += test_case<long double, M>( 700.0, 720.0, 0.1, 0 );
  //  tests += test_case<long double, M>( -1000, 1000, 0.1, 0 );
  //  tests += test_case<long double, M>( -1'000'000'000, 0, 1000, 1 );
  //  tests += test_case<long double, M>( -1'000'000, 0, 1, 2 );
  //  tests += test_case<long double, M>( -1'000, 0, 0.001, 3 );
  //  tests += test_case<long double, M>( -1, 0, 0.000001, 4 );
  //  tests += test_case<long double, M>( -0.001, 0, 0.000000001, 5 );
  //  tests += test_case<long double, M>( -1.001, -1, 0.000000001, 6 );
  //  tests += test_case<long double, M>( -1'000.001, -1'000, 0.000000001, 7 );
  //  tests += test_case<long double, M>( -1'000'000.001, -1'000'000, 0.000000001, 8 );
  //  tests += test_case<long double, M>( -1'000'000'000.001, -1'000'000'000, 0.000000001, 9 );
  //  tests += test_case<long double, M>( 0, 1, 0.000001, 10 );
  //
  //  tests += test_case<long double, M>( 0, max_exp, 0.001, 11 );
  //  tests += test_case<long double, M>( max_exp - 1, max_exp, 0.000001, 12 );

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
