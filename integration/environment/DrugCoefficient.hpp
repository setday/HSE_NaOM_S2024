#pragma once

#include <cmath>
#include <stdexcept>

namespace ADAAI::Integration::Environment
{
  /// \brief A callable object that returns the value of C_D( M ) at the given point M

  struct DrugCoefficient
  {
  private:
    float M;

    static float CD[60]; // is must be filled with values obtained from a graph
                         // (check tg message from Merkin)
    const float d_CD = 0.03f;

    static float slope[60]; // (CD[i + 1] - CD[i]) / (M[i + 1] - M[i])
                            // must be precomputed
    static bool is_initialized;

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
      if ( x < 0.3f )
      {
        throw std::invalid_argument( "h_strange: Invalid argument" );
      }

      if ( x < 0.95f )
      {
        return f_strange( x );
      }
      if ( x > 1.1f )
      {
        return g_strange( x );
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

    void initialize()
    {
      initialize_CD();
      initialize_slope();
    }

  public:
    explicit DrugCoefficient( float MachNumber )
        : M( MachNumber )
    {
      if ( !is_initialized )
      {
        initialize();
        is_initialized = true;
      }
    }

    float operator()() const // M is a Mach number
    {
      int i = ( int ) ( M / d_CD );
      return CD[i] + slope[i] * ( M - d_CD * ( float ) i );
    }
  };

  bool DrugCoefficient::is_initialized = false;
} // namespace ADAAI::Integration::Environment
