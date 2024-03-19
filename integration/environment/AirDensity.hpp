#pragma once

#include <cmath>
#include <stdexcept>

namespace ADAAI::Integration::Environment
{
  const float G_force = 9.80655f;  // gravitational acceleration constant

  /// \brief A callable object that returns the value of \rho(h) at the given point h
  struct AirEnviorment
  {
    struct Layer
    {
      float max_height;
      float min_height;

      float avg_temperature;
      float avg_pressure;
      float avg_r;
    };

  private:
    float h;

    const float R_air   = 287.0528f; // R_air constant

    const Layer layers[4] = {
        // max_height, min_height, avg_temperature, avg_pressure, avg_r
        { 11'000, 0, 288.15f, 101325.0f, 0.0065f },
        { 20'000, 11'000, 216.65f, 22632.3777602f, 0 },
        { 32'000, 20'000, 216.65f, 5475.03644646f, -0.001f },
        { 47'000, 32'000, 228.65f, 868.056901172f, -0.0028f } // should this have a maximum height?
    };

    [[nodiscard]] float computeAtmosphericPressure( float height, int caseNum ) const
    {
      if ( caseNum < 0 || caseNum > 3 )
      {
        throw std::invalid_argument( "atmosphericPressure: Invalid case number" );
      }

      float
          p_l = layers[caseNum].avg_pressure,
          r_l = layers[caseNum].avg_r,
          h_l = layers[caseNum].min_height,
          T_l = layers[caseNum].avg_temperature;

      float
          d_height = height - h_l;

      if ( caseNum == 0 )
      {
        return p_l * std::exp( -G_force * d_height / ( R_air * T_l ) );
      }

      return p_l * std::exp( G_force / ( R_air * r_l ) * std::log( 1 - r_l * d_height / T_l ) );
    }

  public:
    explicit AirEnviorment( float height )
        : h( height )
    {
    }

    [[nodiscard]] float getPressure() const
    {
      for ( int l_id = 0; l_id < 4; l_id++ )
      {
        const Layer& layer = layers[l_id];

        if ( h < layer.min_height || h > layer.max_height )
        {
          continue;
        }

        return computeAtmosphericPressure( h, l_id );
      }

      throw std::invalid_argument( "AirDensity: Invalid height" );
    }

    [[nodiscard]] float getDensity() const
    {
      for ( int l_id = 0; l_id < 4; l_id++ )
      {
        const Layer& layer = layers[l_id];

        if ( h < layer.min_height || h > layer.max_height )
        {
          continue;
        }

        return computeAtmosphericPressure( h, l_id ) /
               ( R_air * ( layer.avg_temperature - layer.avg_r * ( h - layer.min_height ) ) );
      }

      throw std::invalid_argument( "AirDensity: Invalid height" );
    }
  };
} // namespace ADAAI::Integration::Environment
