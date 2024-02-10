#pragma once

#include <cmath>
#include <cstdint>

namespace ADAAI::MATH
{
  template<typename F>
  inline F ldexp( F x, int exp )
  {
    return std::ldexp( x, exp );
  }
  template<>
  inline float ldexp( float x, int exp ) {
      const int exp_shift = 23;
      const int exp_mask = 0xFF;
      const int exp_max = 0xFF;

      int32_t bit_float = *(int*)&x;

      int x_exp = (int)(bit_float >> exp_shift & exp_mask);

      bit_float ^= x_exp << exp_shift;

      x_exp += exp;
      if (x_exp >= exp_max) {
          return std::numeric_limits<float>::infinity();
      }
      if (x_exp <= 0) {
          return 0.0f;
      }

      bit_float |= x_exp << exp_shift;

      return *(float*)&bit_float;
  };
  template<>
  inline double ldexp( double x, int exp ) {
      const int exp_shift = 52;
      const int exp_mask = 0x7FF;
      const int exp_max = 0x7FF;

      int64_t bit_double = *(int64_t*)&x;

      int x_exp = (int)(bit_double >> exp_shift & exp_mask);

      bit_double ^= (int64_t)x_exp << exp_shift;

      x_exp += exp;
      if (x_exp >= exp_max) {
          return std::numeric_limits<double>::infinity();
      }
      if (x_exp <= 0) {
          return 0.0;
      }

      bit_double |= (int64_t)x_exp << exp_shift;

      return *(double*)&bit_double;
  };
  template<>
  inline long double ldexp( long double x, int exp ) {
      const int exp_shift = 48; // trick: ld is 80 bit number, but we can get only 64 first bits and change them
      const int exp_mask = 0x7FFF;
      const int exp_max = 0x7FFF;

      x = 2.0;

      int64_t bit_long_double = *(int64_t*)&x;

      int x_exp = (int)(bit_long_double >> exp_shift & exp_mask);

      bit_long_double ^= (int64_t)x_exp << exp_shift;

      x_exp += exp;
      if (x_exp >= exp_max) {
          return std::numeric_limits<long double>::infinity();
      }
      if (x_exp <= 0) {
          return 0.0;
      }

      bit_long_double |= (int64_t)x_exp << exp_shift;

      return *(long double*)&bit_long_double;
  };
} // namespace ADAAI::CONST
