#pragma once

#include <numbers>
#include <vector>
#include <cmath>

namespace ADAAI::CONST
{  
  template<typename T>
  constexpr inline T LOG2E;
  template<>
  constexpr inline float LOG2E<float> = std::numbers::log2e_v<float>;
  template<>
  constexpr inline double LOG2E<double> = std::numbers::log2e_v<double>;
  template<>
  constexpr inline long double LOG2E<long double> = std::numbers::log2e_v<long double>;

  template<typename T>
  constexpr inline T LN2;
  template<>
  constexpr inline float LN2<float> = std::numbers::ln2_v<float>;
  template<>
  constexpr inline double LN2<double> = std::numbers::ln2_v<double>;
  template<>
  constexpr inline long double LN2<long double> = std::numbers::ln2_v<long double>;

  template<typename T>
  constexpr inline T EPS;
  template<>
  constexpr inline float EPS<float> = std::numeric_limits<float>::epsilon();
  template<>
  constexpr inline double EPS<double> = std::numeric_limits<double>::epsilon();
  template<>
  constexpr inline long double EPS<long double> = std::numeric_limits<long double>::epsilon();

  constexpr float scale = 10;
  template<typename T>
  constexpr inline T DELTA;
  template<>
  constexpr inline float DELTA<float> = scale * EPS<float>;
  template<>
  constexpr inline double DELTA<double> = scale * EPS<double>;
  template<>
  constexpr inline long double DELTA<long double> = scale * EPS<long double>;

  constexpr float bound_scale = 535;
  template<typename T>
  constexpr inline T BOUND;
  template<>
  constexpr inline float BOUND<float> = bound_scale * EPS<float>;
  template<>
  constexpr inline double BOUND<double> = bound_scale * EPS<double>;
  template<>
  constexpr inline long double BOUND<long double> = bound_scale * EPS<long double>;

  template<typename T>
  constexpr inline T SQRT2;
  template<>
  constexpr inline float SQRT2<float> = std::numbers::sqrt2_v<float>;
  template<>
  constexpr inline double SQRT2<double> = std::numbers::sqrt2_v<double>;
  template<>
  constexpr inline long double SQRT2<long double> = std::numbers::sqrt2_v<long double>;

  // sizes of P_TERMS + Q_TERMS for T float 9, double 15, long double 17
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
} // namespace ADAAI::CONST
