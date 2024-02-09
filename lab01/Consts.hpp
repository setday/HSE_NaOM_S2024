#pragma once

#include <cfloat>
#include <numbers>
#include <vector>

namespace ADAAI::CONST
{
  template<typename F>
  constexpr inline F LOG2E;
  template<>
  constexpr inline float LOG2E<float> = std::numbers::log2e_v<float>;
  template<>
  constexpr inline double LOG2E<double> = std::numbers::log2e_v<double>;
  template<>
  constexpr inline long double LOG2E<long double> = std::numbers::log2e_v<long double>;

  template<typename F>
  constexpr inline F LN2;
  template<>
  constexpr inline float LN2<float> = std::numbers::ln2_v<float>;
  template<>
  constexpr inline double LN2<double> = std::numbers::ln2_v<double>;
  template<>
  constexpr inline long double LN2<long double> = std::numbers::ln2_v<long double>;

  template<typename F>
  constexpr inline F EPS;
  template<>
  constexpr inline float EPS<float> = std::numeric_limits<float>::epsilon();
  template<>
  constexpr inline double EPS<double> = std::numeric_limits<double>::epsilon();
  template<>
  constexpr inline long double EPS<long double> = std::numeric_limits<long double>::epsilon();

  constexpr float scale = 10;
  template<typename F>
  constexpr inline F DELTA;
  template<>
  constexpr inline float DELTA<float> = scale * EPS<float>;
  template<>
  constexpr inline double DELTA<double> = scale * EPS<double>;
  template<>
  constexpr inline long double DELTA<long double> = scale * EPS<long double>;

  template<typename F>
  constexpr inline F SQRT2;
  template<>
  constexpr inline float SQRT2<float> = std::numbers::sqrt2_v<float>;
  template<>
  constexpr inline double SQRT2<double> = std::numbers::sqrt2_v<double>;
  template<>
  constexpr inline long double SQRT2<long double> = std::numbers::sqrt2_v<long double>;

  // sizes of P_TERMS + Q_TERMS for F float 9, double 15, long double 17
  template<typename F>
  constexpr inline F* P_TERMS;
  template<>
  constexpr inline float P_TERMS<float>[] = { -1.0, -90.0, -4410.0, -152880.0, -4127760.0, -90810720.0, -1664863200.0, -25686460800.0, -333923990400.0, -3636061228800.0, -32724551059200.0, -237996734976000.0, -1348648164864000.0, -5602076992512000.0, -15205637551104000.0, -20274183401472000.0 };

  template<>
  constexpr inline double P_TERMS<double>[] = { -1.0, -90.0, -4410.0, -152880.0, -4127760.0, -90810720.0, -1664863200.0, -25686460800.0, -333923990400.0, -3636061228800.0, -32724551059200.0, -237996734976000.0, -1348648164864000.0, -5602076992512000.0, -15205637551104000.0, -20274183401472000.0 };

  template<>
  constexpr inline long double P_TERMS<long double>[] = { -1.0, -90.0, -4410.0, -152880.0, -4127760.0, -90810720.0, -1664863200.0, -25686460800.0, -333923990400.0, -3636061228800.0, -32724551059200.0, -237996734976000.0, -1348648164864000.0, -5602076992512000.0, -15205637551104000.0, -20274183401472000.0 };

  template<typename F>
  constexpr inline F* Q_TERMS;
  template<>
  constexpr inline float Q_TERMS<float>[] = { 10897286400.0, -871782912000.0, +29640619008000.0, -533531142144000.0, +5068545850368000.0, -20274183401472000.0 };
  template<>
  constexpr inline double Q_TERMS<double>[] = { 10897286400.0, -871782912000.0, +29640619008000.0, -533531142144000.0, +5068545850368000.0, -20274183401472000.0 };
  template<>
  constexpr inline long double Q_TERMS<long double>[] = { 10897286400.0, -871782912000.0, +29640619008000.0, -533531142144000.0, +5068545850368000.0, -20274183401472000.0 };
} // namespace ADAAI::CONST
