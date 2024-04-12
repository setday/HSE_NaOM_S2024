#pragma once

#include <cmath>
#include <stdexcept>

namespace ADAAI::Integration::Environment
{
  /// \brief A callable object that returns the value of C_D( M ) at the given point M

  struct DrugCoefficient
  {
  private:
    double M;

    static double CD[60]; // is must be filled with values obtained from a graph
                          // (check tg message from Merkin)
    const double d_CD = 0.03;

    static double slope[60]; // (CD[i + 1] - CD[i]) / (M[i + 1] - M[i])
                             // must be precomputed
    static bool is_initialized;

    /// \brief A strange function that is used to compute CD
    /// \file See ./../assets/Prec.png

    [[nodiscard]] static double g_strange( double x )
    {
      return 1.0 / ( x + 1.1 ) - 0.04;
    }

    [[nodiscard]] static double f_strange( double x )
    {
      return std::pow( 1.0 / ( 2.05 - x ), 20.0 ) + 0.1;
    }

    [[nodiscard]] static double h_strange( double x )
    {
      if ( x < 0.3 )
      {
        throw std::invalid_argument( "h_strange: Invalid argument" );
      }

      if ( x < 0.95 )
      {
        return f_strange( x );
      }
      if ( x > 1.1 )
      {
        return g_strange( x );
      }

      return std::min( g_strange( x ), f_strange( x ) );
    }

    void initialize_CD() const
    {
      for ( int i = 0; i < 60; i++ )
      {
        CD[i] = h_strange( d_CD * ( double ) i + 0.4 );
      }
    }

    void initialize_slope() const
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
    explicit DrugCoefficient( double MachNumber )
        : M( MachNumber )
    {
      if ( !is_initialized )
      {
        initialize();
        is_initialized = true;
      }
    }

    double operator()() const // M is a Mach number
    {
      int i = ( int ) ( M / d_CD );
      return CD[i] + slope[i] * ( M - d_CD * ( double ) i );
    }
  };

  bool DrugCoefficient::is_initialized = false;
} // namespace ADAAI::Integration::Environment
