#pragma once

#include "../../utils/Consts.hpp"

namespace ADAAI::Exp::Core::Taylor
{
  template<typename T>
  constexpr inline std::size_t MakeTaylorOrder()
  {
    T term = CONST::SQRT2<T>;

    for ( std::size_t i = 1; i < 1000; ++i )
    {
      term *= CONST::LN2<T> * 0.5 / i;
      if ( term < CONST::DELTA<T> )
        return i + 2; // if you'd return i or even i - 1 precision would drop
    }

    throw std::runtime_error( "Taylor order not found" );
  }

  template<typename T>
  constexpr std::size_t N = MakeTaylorOrder<T>(); // this is required number of Tailor terms for type T

  /// \brief Computes exp(x) using Taylor series
  /// \example \code Exp_Taylor( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \param x - Value to compute
  /// \return e^x
  template<typename T>
    requires std::is_floating_point_v<T>
  constexpr T Exp_Taylor( T x )
  {
    T result = 1, term = 1;

    for ( std::size_t n = 1; n < N<T>; ++n )
    {
      term = term * x / n;
      result += term;
    }

    return result;
  }
} // namespace ADAAI::Exp::Core::Taylor
