#pragma once

#include <cmath>
#include <iomanip>

#include "../lab01/Consts.hpp"

/// \brief Namespace for utility functions
/// \details Contains functions for testing and other purposes
namespace ADAAI::Utils
{
  template<typename T>
  struct CheckObjectBase
  {
    bool passed = true;

    std::size_t test_case_number = UINT_MAX;

    std::size_t tests_number = 0;
    std::size_t fails_count  = 0;

    T first_fail = 0;

    std::string test_data;

    virtual bool check_function( T value )
    {
      return true;
    }

    virtual void print_data( std::ostream& os ) const
    {
    }
  };

  /// \brief Tests if two functions are equal for a given value
  /// \example \code assert( ( adaptive_compare<float, ADAAI::Exp, std::exp>( x ) ) ); \endcode
  /// \tparam T - Type of the value
  /// \tparam MimicFunction - Function to adaptive_compare
  /// \tparam RealFunction - Function to compare with
  /// \param x - Value to adaptive_compare
  /// \return True if functions are close enough, false otherwise
  template<typename T, T MimicFunction( T ), T RealFunction( T )>
  T adaptive_compare( T x )
  {
    T got      = MimicFunction( x );
    T expected = RealFunction( x );

    // Checking for special cases
    if ( std::isnan( got ) )
    {
      return std::isnan( expected ) ? 0.0 : std::numeric_limits<T>::quiet_NaN();
    }
    if ( std::isinf( got ) )
    {
      return std::isinf( expected ) ? 0.0 : std::numeric_limits<T>::infinity();
    }

    T eps  = std::numeric_limits<T>::epsilon();
    T diff = std::abs( got - expected );

    if ( x <= 0 )
    {
      return diff / eps;
    }
    return diff / expected / eps;
  }

  /// \brief Tests a range of values with a given step
  /// \example \code range_check<float, ExpTripleCheckObject>( -100.0, 100.0, 0.1 ); \endcode
  /// \tparam T - Type of the value
  /// \tparam CheckObject - An object class with check_function
  /// \param left - Left bound of the range
  /// \param right - Right bound of the range
  /// \param step - Step of the range
  /// \param break_on_fail - If true, stops testing after the first fail
  /// \return CheckObject object with test results
  template<typename T, typename CheckObject>
    requires std::is_base_of_v<CheckObjectBase<T>, CheckObject>
  CheckObject range_check( T left, T right, T step, bool break_on_fail = false )
  {
    static_assert( std::is_base_of_v<CheckObjectBase<T>, CheckObject>, "CheckObject must be derived from CheckObjectBase" );

    CheckObject result;

    result.test_data = "Range check in [" + std::to_string( left ) + ", " + std::to_string( right ) + "] with step " + std::to_string( step );

    for ( T value = left; value <= right; value += step )
    {
      result.tests_number++;

      if ( !result.check_function( value ) )
      {
        if ( result.passed )
        {
          result.first_fail = value;
          result.passed     = false;

          if ( break_on_fail )
          {
            break;
          }
        }

        result.fails_count++;
      }
    }

    return result;
  }

  /// \brief Tests an array of values
  /// \example \code array_check<long double, ExpTripleCheckObject>( test_set , 11 ); \endcode
  /// \tparam T - Type of the value
  /// \tparam CheckFunction - A tester function
  /// \param array - Array of values
  /// \param size - Size of the array
  /// \param break_on_fail - If true, stops testing after the first fail
  /// \return CheckObject object with test results
  template<typename T, typename CheckObject>
    requires std::is_base_of_v<CheckObjectBase<T>, CheckObject>
  CheckObject array_check( T* array, std::size_t size, bool break_on_fail = false )
  {
    CheckObject result;

    result.test_data = "Array check of size " + std::to_string( size );

    for ( std::size_t i = 0; i < size; i++ )
    {
      T value = array[i];

      result.tests_number++;

      if ( !result.check_function( value ) )
      {
        if ( result.passed )
        {
          result.first_fail = value;
          result.passed     = false;

          if ( break_on_fail )
          {
            break;
          }
        }

        result.fails_count++;
      }
    }

    return result;
  }

  /// \brief Override of << operator for cout of CheckObjectBase
  /// \tparam T - Type of the value
  /// \param os - Output stream
  /// \param result - CheckObjectBase to cout
  /// \return Output stream
  template<typename T>
  std::ostream& operator<<( std::ostream& os, const CheckObjectBase<T>& result )
  {
    os << "=== Test case " << ( result.test_case_number == UINT_MAX ? 0 : result.test_case_number )
       // << " " << ( result.passed ? "PASSED!" : "FAILED!" )
       << " ===\n";
    os << "=> Test data: " << result.test_data << '\n';
    result.print_data( os );
    os << "===--===---===---===--===";
    return os;
  }
} // namespace ADAAI::Utils
