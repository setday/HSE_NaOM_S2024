#pragma once

#include <cmath>
#include <stdexcept>
#include <type_traits>

namespace ADAAI::Integration
{
  /// \brief A callable object that returns the value of \rho(h) at the given point h
  struct AirDensity
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

    const float G_force = 9.80655f;  // gravitational acceleration constant
    const float R_air   = 287.0528f; // R_air constant

    const Layer layers[4] = {
        // max_height, min_height, avg_temperature, avg_pressure, avg_r
        { 11'000, 0, 288.15f, 101325.0f, 0.0065f },
        { 20'000, 11'000, 216.65f, 22632.3777602f, 0 },
        { 32'000, 20'000, 216.65f, 5475.03644646f, -0.001f },
        { 47'000, 32'000, 228.65f, 868.056901172f, -0.0028f } // should this have a maximum height?
    };

  public:
    explicit AirDensity( float height )
        : h( height )
    {
    }

    [[nodiscard]] float atmosphericPressure( float height, int caseNum ) const
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

    float operator()() const
    {
      for ( int l_id = 0; l_id < 4; l_id++ )
      {
        const Layer& layer = layers[l_id];

        if ( h < layer.min_height || h > layer.max_height )
        {
          continue;
        }

        return atmosphericPressure( h, l_id ) /
               ( R_air * ( layer.avg_temperature - layer.avg_r * ( h - layer.min_height ) ) );
      }

      throw std::invalid_argument( "AirDensity: Invalid height" );
    }
  };

  /// \brief A callable object that returns the value of C_D( M ) at the given point M

  struct DrugCoefficient
  {
  private:
    float M;

    float CD[60] {}; // is must be filled with values obtained from a graph
                     // (check tg message from Merkin)
    float d_CD = 0.03f;

    float slope[60] {}; // (CD[i + 1] - CD[i]) / (M[i + 1] - M[i])
                        // must be precomputed

    /// \brief A strange function that is used to compute CD
    /// \file See ./../assets/Prec.png

    [[nodiscard]] static float g_strange( float x )
    {
      return 1.0f / ( x + 1.1f ) - 0.04f;
    }

    [[nodiscard]] static float f_strange( float x )
    {
      return std::pow( 1.0f / ( 2.05f - x ), 20.0f ) + 0.1f;
    }

    [[nodiscard]] static float h_strange( float x )
    {
      if ( x < 0.3f || x > 2.3f )
      {
        throw std::invalid_argument( "h_strange: Invalid argument" );
      }

      return std::min( g_strange( x ), f_strange( x ) );
    }

    void initialize_CD()
    {
      for ( int i = 0; i < 60; i++ )
      {
        CD[i] = h_strange( d_CD * ( float ) i + 0.4f );
      }
    }

    void initialize_slope()
    {
      for ( int i = 0; i < 59; i++ )
      {
        slope[i] = ( CD[i + 1] - CD[i] ) / d_CD;
      }
    }

  public:
    explicit DrugCoefficient( float MachNumber )
        : M( MachNumber )
    {
      initialize_CD();
      initialize_slope();
    }

    float operator()() const // M is a Mach number
    {
      int i = ( int ) ( M / d_CD );
      return CD[i] + slope[i] * ( M - d_CD * ( float ) i );
    }
  };

  /// \brief Computes the value of Q (given M, y and v_x, v_y)
  float AeroDynamicForce( float M, float y, float v_x, float v_y )
  {
    static float d = 0.216;              // calibre (diameter) in meters
    static float S = M_PI * d * d / 4.0; // cross-sectional area
    float        v = std::sqrt( v_x * v_x + v_y * v_y );

    return DrugCoefficient { M }() * AirDensity { y }() * v * v * S / 2.0;
  }
} // namespace ADAAI::Integration
