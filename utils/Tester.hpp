#pragma once

#include <cmath>
#include <iomanip>

#include "../lab01/Consts.hpp"

/// \brief Namespace for utility functions
/// \details Contains functions for testing and other purposes
namespace ADAAI::Utils
{
  template<typename T>
  struct TestResult
  {
    bool passed = true;

    std::size_t test_case_number = UINT_MAX;
    std::size_t tests_number     = 0;
    std::size_t fails_count      = 0;

    T first_fail = 0;

    T min_eps = -1;

    void update( T value, bool test_passed )
    {
      tests_number++;

      if ( !test_passed )
      {
        if ( passed )
        {
          first_fail = value;
          passed     = false;
        }

        fails_count++;
      }
    }
  };

  template<typename T>
    requires std::is_floating_point_v<T>
  struct CheckResult
  {
    bool passed = true;
    T    error  = 0;

    void update( T inp )
    {
      if ( !std::isinf<T>( inp ) )
        error = std::max( error, inp );
    }
  };

  /// \brief Tests if two functions are equal for a given value
  /// \example \code assert( ( adaptive_compare<float, ADAAI::Exp, std::exp>( x ) ) ); \endcode
  /// \tparam T - Type of the value
  /// \tparam MimicFunction - Function to adaptive_compare
  /// \tparam RealFunction - Function to compare with
  /// \param x - Value to adaptive_compare
  /// \return CheckResult structure
  template<typename T, T MimicFunction( T ), T RealFunction( T )>
  CheckResult<T> adaptive_compare( T x )
  {
    T got      = MimicFunction( x );
    T expected = RealFunction( x );

    // Checking for special cases
    if ( std::isnan( got ) )
    {
      return { std::isnan( expected ), 0.0 };
    }
    if ( std::isinf( got ) )
    {
      return { std::isinf( expected ), 0.0 };
    }

    T diff = std::abs( got - expected );

    if ( x <= 0 )
    {
      return { diff < ADAAI::CONST::BOUND<T>, diff / ADAAI::CONST::EPS<T> };
    }
    return { diff < ADAAI::CONST::BOUND<T> * expected, diff / expected / ADAAI::CONST::EPS<T> };
  }

  /// \brief Tests a range of values with a given step
  /// \example \code range_check<float, exp_triple_check>( -100.0, 100.0, 0.1 ); \endcode
  /// \tparam T - Type of the value
  /// \tparam CheckFunction - A tester function
  /// \param left - Left bound of the range
  /// \param right - Right bound of the range
  /// \param step - Step of the range
  /// \param break_on_fail - If true, stops testing after the first fail
  /// \return TestResult structure
  template<typename T, bool CheckFunction( T )>
  TestResult<T> range_check( T left, T right, T step, bool break_on_fail = false )
  {
    TestResult<T> result;

    for ( T value = left; value <= right; value += step )
    {
      bool passed = CheckFunction( value );
      result.update( value, passed );
      if ( !passed && break_on_fail )
      {
        break;
      }
    }

    return result;
  }

  /// \brief Tests an array of values
  /// \example \code array_check<long double, exp_triple_check>( test_set , 11 ); \endcode
  /// \tparam T - Type of the value
  /// \tparam CheckFunction - A tester function
  /// \param array - Array of values
  /// \param size - Size of the array
  /// \param break_on_fail - If true, stops testing after the first fail
  /// \return TestResult structure
  template<typename T, bool CheckFunction( T )>
  TestResult<T> array_check( T* array, std::size_t size, bool break_on_fail = false )
  {
    TestResult<T> result;

    for ( std::size_t i = 0; i < size; i++ )
    {
      T value = array[i];

      bool passed = CheckFunction( value );
      result.update( value, passed );
      if ( !passed && break_on_fail )
      {
        break;
      }
    }

    return result;
  }

  /// \brief Override of << operator for cout of TestResult
  /// \tparam T - Type of the value
  /// \param os - Output stream
  /// \param result - TestResult to cout
  /// \return Output stream
  template<typename T>
  std::ostream& operator<<( std::ostream& os, const TestResult<T>& result )
  {
    os << "=== Test case " << ( result.test_case_number == UINT_MAX ? 0 : result.test_case_number )
       << " " << ( result.passed ? "PASSED!" : "FAILED!" ) << " ===\n";
    os << "=> Tests passed: " << result.tests_number - result.fails_count << '/' << result.tests_number << '\n';
    //        if ( !result.passed )
    //        {
    //          os << "=! First fail: " << result.first_fail << '\n';
    //    }
    //    if ( result.min_eps != -1 )
    //    {
    //      os << "=> Minimal number of epsilons: " << result.min_eps << '\n';
    //    }
    //    os << "===--===---===---===--===";
    return os;
  }
} // namespace ADAAI::Utils
