#pragma once

#include <cmath>
#include <stdexcept>

namespace ADAAI::Integration::Environment
{
  const double G_force = 9.80655; // gravitational acceleration constant

  /// \brief A callable object that returns the value of \rho(h) at the given point h
  struct AirEnvironment
  {
    struct Layer
    {
      double max_height;
      double min_height;

      double avg_temperature;
      double avg_pressure;
      double avg_r;
    };

  private:
    double h;

    const double R_air = 287.0528; // R_air constant

    const Layer layers[4] = {
        // max_height, min_height, avg_temperature, avg_pressure, avg_r
        { 11'000, 0, 288.15, 101325.0, 0.0065 },
        { 20'000, 11'000, 216.65, 22632.3777602, 0 },
        { 32'000, 20'000, 216.65, 5475.03644646, -0.001 },
        { 47'000, 32'000, 228.65, 868.056901172, -0.0028 } };

    [[nodiscard]] double computeAtmosphericPressure( double height, int caseNum ) const
    {
      if ( caseNum < 0 || caseNum > 3 )
      {
        throw std::invalid_argument( "atmosphericPressure: Invalid case number" );
      }

      double
          p_l = layers[caseNum].avg_pressure,
          r_l = layers[caseNum].avg_r,
          h_l = layers[caseNum].min_height,
          T_l = layers[caseNum].avg_temperature;

      double
          d_height = height - h_l;

      if ( caseNum == 1 )
      {
        return p_l * std::exp( -G_force * d_height / ( R_air * T_l ) );
      }

      return p_l * std::exp( G_force / ( R_air * r_l ) * std::log( 1 - r_l * d_height / T_l ) );
    }

  public:
    explicit AirEnvironment( double height )
        : h( height )
    {
    }

    [[nodiscard]] double getPressure() const
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

      if ( h < 0 )
      {
        throw std::invalid_argument( "AirDensity: Invalid height" );
      }

      return computeAtmosphericPressure( h, 3 );
    }

    [[nodiscard]] double getDensity() const
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

      if ( h < 0 )
      {
        throw std::invalid_argument( "AirDensity: Invalid height" );
      }

      return computeAtmosphericPressure( h, 3 ) /
             ( R_air * ( layers[3].avg_temperature - layers[3].avg_r * ( h - layers[3].min_height ) ) );
    }
  };
} // namespace ADAAI::Integration::Environment
