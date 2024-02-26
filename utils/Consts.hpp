#pragma once

#include <numbers>
#include <vector>

namespace ADAAI::CONST
{
  const inline long double EXP_OF_PI = std::exp( M_PI ); // can't be constexpr using std::exp

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
} // namespace ADAAI::CONST
