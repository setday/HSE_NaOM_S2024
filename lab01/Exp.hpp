#pragma once

#include <climits>

#include "../utils/Math.hpp"
#include "Consts.hpp"

namespace ADAAI
{

  enum class Method : int
  {
    Taylor = 1,
    Pade   = 2,
  };

  /// \brief Namespace for core functions
  /// \details Contains functions for computing core functions
  namespace core
  {

    template<typename T>
    constexpr inline std::size_t MakeTaylorOrder()
    {
      T term = CONST::SQRT2<T>;
      for ( std::size_t i = 1; i < 1000; ++i )
      {
        term *= CONST::LN2<T> * 0.5 / i;
        if ( term < CONST::DELTA<T> )
        {
          return i + 2; // if you'd return i or even i - 1 precision would drop
        }
      }
      assert( false );
    }

    template<typename T>
    constexpr std::size_t N = MakeTaylorOrder<T>(); // this is required number of Tailor terms for T type

    /// \brief Computes exp(x) using Taylor series
    /// \example \code Exp_( 0.1 ); \endcode
    /// \tparam T - Floating point type
    /// \tparam M - Method to calculate exp
    /// \param x - Value to compute
    /// \return e^x
    template<typename T, Method M = Method::Taylor>
      requires std::is_floating_point_v<T>
    constexpr T Exp_( T x )
    {
      if ( M == Method::Taylor )
      {
        T result = 1, term = 1;
        for ( std::size_t n = 1; n < N<T>; ++n )
        {
          term = term * x / n;
          result += term;
        }
        return result;
      }

      if ( M == Method::Pade )
      {
        T numerator   = 0;
        T denominator = 0;

        for ( const auto& term : CONST::P_TERMS<T> )
        {
          numerator = x * numerator + term;
        }

        for ( const auto& term : CONST::Q_TERMS<T> )
        {
          denominator = x * denominator + term;
        }

        return numerator / denominator;
      }
      assert( false );
    }

  } // namespace core

  /// \brief Computes exp(x)
  /// \example \code Exp( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \tparam M - Method to calculate exp
  /// \param x - Value to compute
  /// \return e^x
  template<typename T, Method M = Method::Taylor>
    requires std::is_floating_point_v<T>
  constexpr T Exp( T x )
  {
    if ( std::isnan( x ) )
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T y = CONST::LOG2E<T> * x, int_part = 0;
    T frac_part = std::modf( y, &int_part );

    if ( int_part < INT_MIN )
    {
      return 0.0;
    }
    if ( int_part > 11500 ) // this will be overflow for any floating type
    {
      return std::numeric_limits<T>::infinity();
    }

    int n = int( int_part ); // generic case now: e^x = 2^n * e^x2

    //    if ( frac_part > 0.5 ) // lowers the precision
    //    {
    //      n++;
    //      frac_part -= 1;
    //    }

    T x2 = CONST::LN2<T> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    T E2 = core::Exp_<T, M>( x2 );
    T En = std::pow<T>( 2.0, n ); // should've been equivalent to std::ldexp(1.0, n);
    return E2 * En;
  }
} // namespace ADAAI
