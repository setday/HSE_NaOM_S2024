#pragma once

#include <cmath>

#include "../lab01/Consts.hpp"

/// \brief Namespace for utility functions
/// \details Contains functions for testing and other purposes
namespace ADAAI::Utils
{
  /// \brief Tests if two functions are equal for a given value
  /// \example \code assert( ( adaptive_compare<float, ADAAI::Exp, std::exp>( x ) ) ); \endcode
  /// \tparam T - Type of the value
  /// \tparam MimicFunction - Function to adaptive_compare
  /// \tparam RealFunction - Function to compare with
  /// \param x - Value to adaptive_compare
  /// \return True if functions are close enough, false otherwise
  template<typename T, T MimicFunction( T ), T RealFunction( T )>
  bool adaptive_compare( T x )
  {
    T got      = MimicFunction( x );
    T expected = RealFunction( x );

    // Checking for special cases
    if ( std::isnan( got ) )
    {
      return std::isnan( expected );
    }
    if ( std::isinf( got ) )
    {
      return std::isinf( expected );
    }

    T diff = std::abs( got - expected );
    T eps  = 304 * ADAAI::CONST::EPS<T>;

    if ( x <= 0 )
    {
      return diff < eps;
    }
    return diff < eps * expected;
  }
} // namespace ADAAI::Utils
