#include <iostream>

#include "TestObjects.hpp"

using namespace ADAAI::Exp::Tests;

template<ADAAI::Exp::Method M = ADAAI::Exp::Method::Taylor>
bool test_case( long double left, long double right, long double step )
{
  auto result = range_check<long double, ExpTripleCheckObject<M>>( left, right, step );
  std::cout << result << "\n\n";

  return result.passed;
}

template<ADAAI::Exp::Method M = ADAAI::Exp::Method::Taylor>
bool test_case( std::vector<long double> array )
{
  auto result = array_check<long double, ExpTripleCheckObject<M>>( array.data(), array.size() );
  std::cout << result << "\n\n";

  return result.passed;
}

/// \brief Tests the standard values of the exp function (random + [-100, 100] + limits)
/// \details Average test time: 0.5s
template<ADAAI::Exp::Method M = ADAAI::Exp::Method::Taylor>
void exp_standard_tests()
{
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
template<ADAAI::Exp::Method M = ADAAI::Exp::Method::Taylor>
void exp_range_tests()
{
  test_case<M>( -300, 1000, 0.001 );
}
