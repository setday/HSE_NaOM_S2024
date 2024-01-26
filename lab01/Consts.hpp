#pragma once
#include <cfloat>
#include <numbers>

namespace ADAAI::CONST {
template <typename F> constexpr inline F LOG2E;
template <> constexpr inline float LOG2E<float> = std::numbers::log2e_v<float>;
template <>
constexpr inline double LOG2E<double> = std::numbers::log2e_v<double>;
template <>
constexpr inline long double LOG2E<long double> =
    std::numbers::log2e_v<long double>;

template <typename F> constexpr inline F LN2;
template <> constexpr inline float LN2<float> = std::numbers::ln2_v<float>;
template <> constexpr inline double LN2<double> = std::numbers::ln2_v<double>;
template <>
constexpr inline long double LN2<long double> =
    std::numbers::ln2_v<long double>;

template <typename F> constexpr inline F EPS;
template <> constexpr inline float EPS<float> = FLT_EPSILON;
template <> constexpr inline double EPS<double> = DBL_EPSILON;
template <> constexpr inline long double EPS<long double> = LDBL_EPSILON;

template <typename F> constexpr inline F DELTA;
template <> constexpr inline float DELTA<float> = 10.0 * EPS<float>;
template <> constexpr inline double DELTA<double> = 10.0 * EPS<double>;
template <>
constexpr inline long double DELTA<long double> = 10.0 * EPS<long double>;

template <typename F> constexpr inline F SQRT2;
template <> constexpr inline float SQRT2<float> = std::numbers::sqrt2_v<float>;
template <>
constexpr inline double SQRT2<double> = std::numbers::sqrt2_v<double>;
template <>
constexpr inline long double SQRT2<long double> =
    std::numbers::sqrt2_v<long double>;

}  // namespace ADAAI::CONST
