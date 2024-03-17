#pragma once

#include <cmath>
#include <type_traits>

namespace ADAAI::Integration
{
  /// \brief A callable object that returns the value of air density at the given point
  template<typename T>
    requires std::is_floating_point_v<T>
  class AirDensity
  {
    const float g     = 9.80655f;  // gravitational acceleration constant
    const float R_air = 287.0528f; // R_air constant

    const int h0 = 0;
    const int h1 = 11'000;
    const int h2 = 20'000;
    const int h3 = 32'000;
    const int h4 = 47'000; // but we won't use this number?

    const float r0 = 0.0065f;
    const float r1 = 0;
    const float r2 = -0.001f;
    const float r3 = -0.0028f;

    const float T0 = 288.15f;
    const float T1 = 216.65f;
    const float T2 = 216.65f;
    const float T3 = 228.65f;

    const float p0 = 101325.0f;
    const float p1 = 22632.3777602f;
    const float p2 = 5475.03644646f;
    const float p3 = 868.056901172f;

    T atmosphericPressure( T h, int caseNum )
    {
      if ( caseNum == 1 )
      {
        return p0 * std::exp( -g * ( h - h1 ) / ( R_air * T1 ) );
      }
      else
      {
        float r_l, h_l, T_l;
        if ( caseNum == 0 )
        {
          r_l = r0;
          h_l = h0;
          T_l = T0;
        }
        else if ( caseNum == 2 )
        {
          r_l = r2;
          h_l = h2;
          T_l = T2;
        }
        else
        { // caseNum == 3
          r_l = r3;
          h_l = h3;
          T_l = T3;
        }
        return p0 * std::exp( -g / ( R_air * r_l ) * std::log( 1 - r_l * ( h - h_l ) / T_l ) );
      }
    }

  public:
    T operator()( T h )
    {
      if ( h >= h0 && h <= h1 )
      {
        return atmosphericPressure( h, 0 ) / ( R_air * ( T0 - r0 * ( h - h0 ) ) );
      }
      else if ( h > h1 && h <= h2 )
      {
        return atmosphericPressure( h, 1 ) / ( R_air * ( T1 - r0 * ( h - h1 ) ) );
      }
      else if ( h > h2 && h <= h3 )
      {
        return atmosphericPressure( h, 2 ) / ( R_air * ( T2 - r2 * ( h - h2 ) ) );
      }
      else if ( h > h3 && h <= h4 )
      {
        return atmosphericPressure( h, 3 ) / ( R_air * ( T3 - r3 * ( h - h3 ) ) );
      }
      return static_cast<T>( -239 ); // error
    }
  };

} // namespace ADAAI::Integration
