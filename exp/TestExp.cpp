#include <cassert>
#include <climits>
#include <iostream>

#include "../utils/Tester.hpp"
#include "Exp.hpp"

using namespace ADAAI::Utils;

template<ADAAI::Method M, typename T>
struct ExpSingleCheckObject : public CheckObjectBase<T>
{
  T error = 0.0;

  bool check_function( T x ) override
  {
    auto check = adaptive_compare<T, ADAAI::Exp<T, M>, std::exp>( x );
    error      = std::max( error, check );

    return check < ADAAI::CONST::BOUND<T>;
  }

  void print_data( std::ostream& os ) const override
  {
    os << "=> Max error: " << error << "\n";
  }
};

template<ADAAI::Method M>
struct ExpTripleCheckObject : public CheckObjectBase<long double>
{
  long double error = 0.0;

  bool check_function( long double x ) override
  {
    auto float_check       = adaptive_compare<float, ADAAI::Exp<float, M>, std::exp>( x );
    auto double_check      = adaptive_compare<double, ADAAI::Exp<double, M>, std::exp>( x );
    auto long_double_check = adaptive_compare<long double, ADAAI::Exp<long double, M>, std::exp>( x );

    auto check_error = long_double_check;
    check_error      = std::max( check_error, ( long double ) float_check );
    check_error      = std::max( check_error, ( long double ) double_check );

    error = std::max( error, ( long double ) long_double_check );

    return check_error < ADAAI::CONST::BOUND<long double>;
  }

  void print_data( std::ostream& os ) const override
  {
    os << "=> Max error: " << error << "\n";
  }
};

template<ADAAI::Method M = ADAAI::Method::Taylor>
bool test_case( long double left, long double right, long double step, std::size_t num = 0 )
{
  auto result             = range_check<long double, ExpTripleCheckObject<M>>( left, right, step );
  result.test_case_number = num;
  std::cout << result << "\n\n";

  return result.passed;
}

template<ADAAI::Method M = ADAAI::Method::Taylor>
bool test_case( std::vector<long double> array, std::size_t num = 0 )
{
  auto result             = array_check<long double, ExpTripleCheckObject<M>>( array.data(), array.size() );
  result.test_case_number = num;
  std::cout << result << "\n\n";

  return result.passed;
}

/// \brief Tests the standard values of the exp function (random + [-100, 100] + limits)
/// \details Average test time: 0.5s
template<ADAAI::Method M = ADAAI::Method::Taylor>
void exp_standard_tests()
{
  std::cout << "Method used: " << ( M == ADAAI::Method::Taylor ? "Taylor" : "Pade" ) << "\n";
  test_case<M>( {
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
                },
                1 );

  test_case<M>( -100.0, 100.0, 0.005, 2 );

  test_case<M>( {
                    std::numeric_limits<long double>::infinity(),
                    -std::numeric_limits<long double>::infinity(),
                    std::numeric_limits<long double>::quiet_NaN(),
                    std::numeric_limits<long double>::signaling_NaN(),
                    std::numeric_limits<long double>::denorm_min(),
                    std::numeric_limits<long double>::min(),
                    std::numeric_limits<long double>::max(),
                    std::numeric_limits<long double>::lowest(),
                },
                3 );
}

/// \brief Tests the range of the exp function
/// \details Average test time: 18s
template<ADAAI::Method M = ADAAI::Method::Taylor>
void exp_range_tests()
{
  long double max_exp = 11356.6;

  // real challenge to beat 4000 * exp
  // test_case<M>( 1000.0, 12000.0, 0.1, 1 );

  test_case<M>( 700.0, 720.0, 0.1, 2 );
  test_case<M>( -1000, 1000, 0.001, 3 );

  // just the limits checking (not really useful)
  // test_case<M>( -1'000'000'000, 0, 1000, 1 );
  // test_case<M>( -1'000'000, 0, 1, 2 );
  // test_case<M>( -1'000, 0, 0.001, 3 );
  test_case<M>( -1, 0, 0.000001, 4 );
  // test_case<M>( -0.001, 0, 0.000000001, 5 );
  // test_case<M>( -1.001, -1, 0.000000001, 6 );
  // test_case<M>( -1'000.001, -1'000, 0.000000001, 7 );
  // test_case<M>( -1'000'000.001, -1'000'000, 0.000000001, 8 );
  // test_case<M>( -1'000'000'000.001, -1'000'000'000, 0.000000001, 9 );
  test_case<M>( 0, 1, 0.000001, 10 );

  // test_case<M>( 0, max_exp, 0.001, 11 );
  // test_case<M>( max_exp - 1, max_exp, 0.000001, 12 );
}

int main()
{
  // exp_standard_tests();
  // exp_range_tests();

  // exp_standard_tests<ADAAI::Method::Pade>();
  // exp_range_tests<ADAAI::Method::Pade>();
  auto res = range_check<double, ExpSingleCheckObject<ADAAI::Method::Chebyshev, double>>( -100, 100, 0.001, false );
  std::cout << res;

  //  std::cout << ADAAI::Exp<double, ADAAI::Method::Chebyshev>( 1 ) << " " << std::exp( 1 );

  return 0;
}
