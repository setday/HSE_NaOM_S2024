#pragma once

#include <climits>

#include "../utils/Consts.hpp"
#include "methods/ChebyshevExponential.hpp"
#include "methods/FourierExponential.hpp"
#include "methods/PadeExponential.hpp"
#include "methods/TaylorExponential.hpp"

namespace ADAAI::Exp
{
  enum class Method : int
  {
    Taylor,
    Pade,
    Chebyshev,
    ChebyshevUnused,
    Fourier,
    FourierUnused,
  };

  /// \brief Namespace for core functions
  /// \details Contains functions for computing core functions
  namespace Core
  {
    /// \brief Computes exp(x) using given method
    /// \example \code Exp_<ADAAI::Exp::Method::Taylor>( 0.1 ); \endcode
    /// \tparam T - Floating point type
    /// \tparam M - Method to calculate exp
    /// \param x - Value to compute
    /// \return e^x
    template<typename T, Method M = Method::Taylor>
      requires std::is_floating_point_v<T>
    constexpr T Exp_( T x )
    {
      switch ( M )
      {
        case Method::Taylor:
        {
          return Taylor::Exp_Taylor( x );
        }
        case Method::Pade:
        {
          return Pade::Exp_Pade( x );
        }
        case Method::Chebyshev:
        {
          return Chebyshev::Exp_Chebyshev( x );
        }
        case Method::ChebyshevUnused:
        {
          return Chebyshev::unused::Exp_Chebyshev( x );
        }
        case Method::Fourier:
        {
          return Fourier::Exp_Fourier( x );
        }
        case Method::FourierUnused:
        {
          return Fourier::unused::Exp_Fourier( x );
        }
        default:
        {
          throw std::invalid_argument( "Invalid method in Exp_" );
        }
      }
    }

  } // namespace Core

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

    if ( int_part < -11500 )
    {
      return 0.0;
    }
    if ( int_part > 11500 ) // this will be overflow for any floating type
    {
      return std::numeric_limits<T>::infinity();
    }

    int n = int( int_part ); // generic case now: e^x = 2^n * e^x2

    if ( frac_part > 0.5 ) // lowers the precision
    {
      n++;
      frac_part -= 1;
    }

    T x2 = CONST::LN2<T> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    T E2 = Core::Exp_<T, M>( x2 );
    T E  = std::ldexp( E2, n );
    return E;
  }
} // namespace ADAAI::Exp
