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

bool normal_tests() {
  auto triple_check = []( auto x )
  {
    return adaptive_compare<float, ADAAI::Exp, std::exp>( x ) &&
           adaptive_compare<double, ADAAI::Exp, std::exp>( x ) &&
           adaptive_compare<long double, ADAAI::Exp, std::exp>( x );
  };

  std::size_t tests = 0;


// =============================================================================


// #define TEST_01
// #define TEST_02
// #define TEST_03
// #define TEST_04
// #define TEST_05
// #define TEST_06
// #define TEST_07
// #define TEST_08
// #define TEST_09

{ // LOW
#ifdef TEST_01
{
  long double left = -1'000'000'000;
  long double right = 0;
  long double step = 1000;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_02
{
  long double left = -1'000'000;
  long double right = 0;
  long double step = 1;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif  

#ifdef TEST_03
{
  long double left = -1'000;
  long double right = 0;
  long double step = 0.001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_04
{
  long double left = -1;
  long double right = 0;
  long double step = 0.000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_05
{
  long double left = -0.001;
  long double right = 0;
  long double step = 0.000000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_06
{
  long double left = -1.001;
  long double right = -1;
  long double step = 0.000000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_07
{
  long double left = -1'000.001;
  long double right = -1'000;
  long double step = 0.000000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_08
{
  long double left = -1'000'000.001;
  long double right = -1'000'000;
  long double step = 0.000000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif

#ifdef TEST_09
{
  long double left = -1'000'000'000.001;
  long double right = -1'000'000'000;
  long double step = 0.000000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif
} // LOW
// passed for 12 * eps


// =============================================================================


// #define TEST_11
#define TEST_12
#define TEST_13

 long double max_exp = 709.7827125;

{ // HIGH
#ifdef TEST_11
{
  long double left = 0;
  long double right = 1;
  long double step = 0.000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "Failed for: " << value << '\n';
      return false; 
    }
    value += step;
  }
}
#endif
// passed for 12 * eps

#ifdef TEST_12
{
  long double left = 0;
  long double right = max_exp;
  long double step = 0.001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "TEST_12 Failed for: " << value << '\n';
      std::cout << std::fixed << std::exp(value) << '\n';
      std::cout << std::fixed << ADAAI::Exp(value) << '\n';
      return false; 
    }
    value += step;
  }
}
#endif
// passed for 304 * eps

#ifdef TEST_13
{
  long double left = max_exp - 1;
  long double right = max_exp;
  long double step = 0.000001;
  long double value = left;
  while(value < right) {
    ++tests;
    if(!triple_check(value)) {
      std::cout << "TEST_13 Failed for: " << value << '\n';
      std::cout << std::fixed << std::exp(value) << '\n';
      std::cout << std::fixed << ADAAI::Exp(value) << '\n';
      return false; 
    }
    value += step;
  }
}
#endif
// passed for 306 * eps
} // HIGH


// =============================================================================


  std::cout << "Success on: " << tests << " tests\n";
  return true;
}

int main()
{
  assert(normal_tests());
  // assert( check_exp() );
  return 0;
}
