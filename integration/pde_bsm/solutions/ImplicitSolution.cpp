#include <cmath>
#include <cstdint>

#include "../AuxiliaryFunctions.hpp"

namespace ADAAI::Integration::PDE_BSM::Implicit
{
  constexpr static uint64_t N = AucRHS::N;
  constexpr static uint64_t M = AucRHS::N;

  double state[ N + 1 ];
  double matrix[ N ][ N ];
  double F_i[ N ];

  static double DT;

  double A_ij( uint64_t i, double tau )
  {
    double a = i * AUX_FUNC::risk_free_interest_rate_function( tau );
    double b = i * AUX_FUNC::sigma_function( tau );
    return b * b / 2 - a;
  }

  double B_ij( uint64_t i, double tau )
  {
    double b = i * AUX_FUNC::sigma_function( tau );
    double c = -AUX_FUNC::risk_free_interest_rate_function( tau );
    return c - b * b - 1 / DT;
  }

  double D_ij( uint64_t i, double tau )
  {
    double a = i * AUX_FUNC::risk_free_interest_rate_function( tau );
    double b = i * AUX_FUNC::sigma_function( tau );
    return b * b / 2 + a;
  }

  void init_matrix( double tau )
  {
    for ( uint64_t i = 1; i <= N - 2; ++i )
    {
      matrix[i][i]     = B_ij( i, tau );
      matrix[i + 1][i] = A_ij( i + 1, tau );
      matrix[i][i + 1] = D_ij( i, tau );
    }
    matrix[N - 1][N - 1] = B_ij( N - 1, tau );
  }

  void init_F_i( double tau )
  {
    for ( uint64_t i = 1; i <= N - 1; ++i )
    {
      F_i[i] = -state[i] / DT;
    }

    F_i[1] -= A_ij( 1, tau ) * 0.0;
    F_i[N - 1] -= D_ij( N - 1, tau ) * AucRHS::S_max - AucRHS::K * std::exp( -AUX_FUNC::get_r_integral( tau ) );

    for ( uint64_t i = 1; i <= N - 2; ++i )
    {
      double coef      = matrix[i + 1][i] / matrix[i][i];
      matrix[i + 1][i] = 0.0;
      matrix[i + 1][i + 1] -= coef * matrix[i][i + 1];

      F_i[i + 1] -= coef * F_i[i];
    }

    for ( uint64_t i = N - 1; i >= 2; --i )
    {
      double coef      = matrix[i - 1][i] / matrix[i][i];
      matrix[i - 1][i] = 0.0;

      F_i[i - 1] -= coef * F_i[i];
    }
  }

  void init_state()
  {
    for ( uint64_t i = 0; i <= N; i++ )
    {
      state[i] = std::max( i * AucRHS::S_max / N - AucRHS::K, 0.0 );
    }
  }

  void recalculate_state( double tau )
  {
    for ( uint64_t i = 1; i <= N - 1; ++i )
    {
      state[i] = F_i[i] / matrix[i][i];
    }

    state[0] = 0.0;
    state[N] = AucRHS::S_max - AucRHS::K * std::exp( -AUX_FUNC::get_r_integral( tau ) );
  }

  double runSolution( double S_tau_max, double tau_max )
  {
    DT      = tau_max / N;

    init_state();

    for ( uint64_t j = 1; j <= M; ++j )
    {
      double tau_j = j * tau_max / M;

      init_matrix( tau_j );
      init_F_i( tau_j );

      recalculate_state( tau_j );
    }

    return AucFunc::get_c( state, S_tau_max );
  }
} // namespace ADAAI::Integration::PDE_BSM::Implicit
