#pragma once

#include <climits>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <vector>

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

    template<typename F>
    constexpr inline std::size_t MakeTaylorOrder()
    {
      F term = CONST::SQRT2<F>;
      for ( std::size_t i = 1; i < 1000; ++i )
      {
        term *= CONST::LN2<F> * 0.5 / i;
        if ( term < CONST::DELTA<F> )
        {
          return i + 2; // if you'd return i or even i - 1 precision would drop
        }
      }
      assert( false );
    }

    template<typename F>
    constexpr std::size_t N = MakeTaylorOrder<F>(); // this is required number of Tailor terms for F type

    /// \brief Computes exp(x) using Taylor series
    /// \example \code Exp_( 0.1 ); \endcode
    /// \tparam F - Floating point type
    /// \param x - Value to compute
    /// \return e^x
    template<Method M = Method::Taylor, typename F>
      requires std::is_floating_point_v<F>
    constexpr F Exp_( F x )
    {
      if ( M == Method::Taylor )
      {
        std::vector<F> terms( N<F>, 1 );

        for ( std::size_t n = 1; n < N<F>; ++n )
        {
          terms[n] = terms[n - 1] * x / n;
        }

        F result = 0;

        for ( std::size_t n = N<F>; n > 0; --n )
        {
          result += terms[n - 1];
        }

        return result;
      }

      if ( M == Method::Pade )
      {
        F numerator   = 0;
        F denominator = 0;

        for ( const auto& term : CONST::P_TERMS<F> )
        {
          numerator = x * numerator + term;
        }

        for ( const auto& term : CONST::Q_TERMS<F> )
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
  /// \tparam F - Floating point type
  /// \param x - Value to compute
  /// \return e^x
  template<typename F, Method M = Method::Taylor>
    requires std::is_floating_point_v<F>
  constexpr F Exp( F x )
  {
    if ( std::isnan( x ) )
    {
      return std::numeric_limits<F>::quiet_NaN();
    }

    F y = CONST::LOG2E<F> * x, int_part = 0;
    F frac_part = std::modf( y, &int_part );

    if ( int_part < INT_MIN )
    {
      return 0.0;
    }
    if ( int_part > INT_MAX )
    {
      return std::numeric_limits<F>::infinity();
    }

    int n  = int( int_part );           // generic case now: e^x = 2^n * e^x2

    //    if ( frac_part > 0.5 )
    //    {
    //      n++;
    //      frac_part -= 1;
    //    }

    F   x2 = CONST::LN2<F> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    F   E2 = core::Exp_<M>( x2 );
    F   En = std::pow<F>( 2.0, n ); // should've been equivalent to std::ldexp(1.0, n);
    return E2 * En;
  }
} // namespace ADAAI
