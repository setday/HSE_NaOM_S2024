#pragma once

#include "../../utils/Consts.hpp"

namespace ADAAI::Exp::Core::Pade
{
  // size of P_TERMS + Q_TERMS is 9 for float, 15 for double, 17 for long double
  template<typename T>
  constexpr inline T* P_TERMS;
  template<>
  constexpr inline float P_TERMS<float>[] = { 1., 25., 300., 2100., 8400., 15120. };
  template<>
  constexpr inline double P_TERMS<double>[] = { -1.0, -64.0, -2016.0, -40320.0, -554400.0, -5322240.0, -34594560.0, -138378240.0, -259459200.0 };
  template<>
  constexpr inline long double P_TERMS<long double>[] = { 1.0, 81.0, 3240.0, 83160.0, 1496880.0, 19459440.0, 181621440.0, 1167566400.0, 4670265600.0, 8821612800.0 };

  template<typename T>
  constexpr inline T* Q_TERMS;
  template<>
  constexpr inline float Q_TERMS<float>[] = { 5, -120, 1260, -6720, 15120 };
  template<>
  constexpr inline double Q_TERMS<double>[] = { 8.0, -504.0, 15120.0, -277200.0, 3326400.0, -25945920.0, 121080960.0, -259459200.0 };
  template<>
  constexpr inline long double Q_TERMS<long double>[] = { 9.0, -720.0, 27720.0, -665280.0, 10810800.0, -121080960.0, 908107200.0, -4151347200.0, 8821612800.0 };

  /// \brief Computes exp(x) using Pade approximation
  /// \example \code Exp_Pade( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \param x - Value to compute
  /// \return e^x
  template<typename T>
    requires std::is_floating_point_v<T>
  constexpr T Exp_Pade( T x )
  {
    T numerator   = 0;
    T denominator = 0;

    for ( const auto& term : P_TERMS<T> )
    {
      numerator = x * numerator + term;
    }

    for ( const auto& term : Q_TERMS<T> )
    {
      denominator = x * denominator + term;
    }

    return numerator / denominator;
  }
} // namespace ADAAI::Exp::Core::Pade
