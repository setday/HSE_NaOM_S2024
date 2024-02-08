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

  // [10, 10]
  // constexpr inline long double P_TERMS[] = {+1, +110, +5940, +205920, +5045040, +90810720, +1210809600, +11762150400, +79394515200, +335221286400, +670442572800};
  // constexpr inline long double Q_TERMS[] = {+1, -110, +5940, -205920, +5045040, -90810720, +1210809600, -11762150400, +79394515200, -335221286400, +670442572800};

  // [19, 1]
  // constexpr inline long double P_TERMS[] = {-1, -38, -1026, -23256, -465120, -8372160, -136745280, -2031644160, -27427196160, -335221286400, -3687434150400, -36203898931200, -313767124070400, -2365321396838400, -15205637551104000, -81096733605888000, -344661117825024000, -1094805903679488000, -2311256907767808000, -2432902008176640000};
  // constexpr inline long double Q_TERMS[] = {121645100408832000, -2432902008176640000};

  // [1, 19]
  // constexpr inline long double P_TERMS[] = {-121645100408832000, -2432902008176640000};
  // constexpr inline long double Q_TERMS[] = {1, -38, +1026, -23256, +465120, -8372160, +136745280, -2031644160, +27427196160, -335221286400, +3687434150400, -36203898931200, +313767124070400, -2365321396838400, +15205637551104000, -81096733605888000, +344661117825024000, -1094805903679488000, +2311256907767808000, -2432902008176640000};

  // [0, 20]
  // constexpr inline long double P_TERMS[] = {0, 2432902008176640000};
  // constexpr inline long double Q_TERMS[] = {1, -20, +380, -6840, +116280, -1860480, +27907200, -390700800, +5079110400, -60949324800, +670442572800, -6704425728000, +60339831552000, -482718652416000, +3379030566912000, -20274183401472000, +101370917007360000, -405483668029440000, +1216451004088320000, -2432902008176640000, +2432902008176640000};

  // [15, 5]
  // constexpr inline long double P_TERMS[] = {-1, -90, -4410, -152880, -4127760, -90810720, -1664863200, -25686460800, -333923990400, -3636061228800, -32724551059200, -237996734976000, -1348648164864000, -5602076992512000, -15205637551104000, -20274183401472000};
  // constexpr inline long double Q_TERMS[] = {10897286400, -871782912000, +29640619008000, -533531142144000, +5068545850368000, -20274183401472000};

  // [5, 15]
  constexpr inline long double P_TERMS[] = {-10897286400, -871782912000, -29640619008000, -533531142144000, -5068545850368000, -20274183401472000};
  constexpr inline long double Q_TERMS[] = {1, -90, +4410, -152880, +4127760, -90810720, +1664863200, -25686460800, +333923990400, -3636061228800, +32724551059200, -237996734976000, +1348648164864000, -5602076992512000, +15205637551104000, -20274183401472000};

} // namespace ADAAI::CONST
