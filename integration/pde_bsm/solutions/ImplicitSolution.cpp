#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../AuxiliaryFunctions.hpp"

namespace ADAAI::Integration::PDE_BSM::Implicit
{
  constexpr static uint64_t N = AucRHS::N;
  constexpr static uint64_t M = AucRHS::N;

  double c_ji[ M + 1 ][ N + 1 ];
  double matrix[ N ][ N ];
  double F_i[ N ];

  constexpr static double DS    = AucRHS::S_max / N;

  static double Tau_max;
  static double DT;

  double tau_j( uint64_t j )
  {
    return j * Tau_max / M;
  }

  double s_i( uint64_t i )
  {
    return i * DS;
  }

  void init_c_ij()
  {
    for ( uint64_t j = 0; j <= M; ++j )
    {
      c_ji[j][0] = 0.0;
      c_ji[j][N] = AucRHS::S_max - AucRHS::K * std::exp( -AUX_FUNC::get_r_integral( tau_j( j ) ) );
    }
    for ( uint64_t i = 0; i <= N; ++i )
    {
      c_ji[0][i] = std::max( i * AucRHS::S_max / N - AucRHS::K, 0.0 );
    }
  }

  double A_ij( uint64_t i, uint64_t j )
  {
    double a = s_i( i ) * AUX_FUNC::risk_free_interest_rate_function( tau_j( j ) ) / DS;
    double b = s_i( i ) * AUX_FUNC::sigma_function( tau_j( j ) ) / DS;
    return b * b / 2 - a;
  }

  double B_ij( uint64_t i, uint64_t j )
  {
    double b = s_i( i ) * AUX_FUNC::sigma_function( tau_j( j ) ) / DS;
    double c = -AUX_FUNC::risk_free_interest_rate_function( tau_j( j ) );
    return c - b * b - 1 / DT;
  }

  double D_ij( uint64_t i, uint64_t j )
  {
    double a = s_i( i ) * AUX_FUNC::risk_free_interest_rate_function( tau_j( j ) ) / DS;
    double b = s_i( i ) * AUX_FUNC::sigma_function( tau_j( j ) ) / DS;
    return b * b / 2 + a;
  }

  void init_matrix( uint64_t j )
  {
    for ( uint64_t i = 1; i <= N - 2; ++i )
    {
      matrix[i][i]     = B_ij( i, j );
      matrix[i + 1][i] = A_ij( i + 1, j );
      matrix[i][i + 1] = D_ij( i, j );
    }
    matrix[N - 1][N - 1] = B_ij( N - 1, j );
  }

  void init_F_i( uint64_t j )
  {
    for ( uint64_t i = 1; i <= N - 1; ++i )
    {
      F_i[i] = -c_ji[j - 1][i] / DT;
    }

    F_i[1] -= A_ij( 1, j ) * c_ji[j][0];
    F_i[N - 1] -= D_ij( N - 1, j ) * c_ji[j][N];

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

  void recalculate_c_ij( uint64_t j )
  {
    for ( uint64_t i = 1; i <= N - 1; ++i )
    {
      c_ji[j][i] = F_i[i] / matrix[i][i];
    }
  }

  double runSolution( double S_tau_max, double tau_max )
  {
    Tau_max = tau_max;
    DT      = Tau_max / N;

    init_c_ij();

    for ( uint64_t j = 1; j <= M; ++j )
    {
      init_matrix( j );

      init_F_i( j );

      recalculate_c_ij( j );
    }

    // for(uint64_t i = 0; i <= N; ++i) {
    //     for(uint64_t j = 0; j <= M; ++j) {
    //         std::printf("%.2g ", c_ji[j][i]);
    //     }
    //     std::printf("\n");
    // }

    uint64_t i = 0;
    while ( s_i( i ) <= S_tau_max )
    {
      ++i;
    }
    --i;
    double S_tau = S_tau_max - i * AucRHS::S_max / N;

    return c_ji[N][i] * ( 1.0 - S_tau ) + c_ji[N][i] * S_tau;
  }
} // namespace ADAAI::Integration::PDE_BSM::Implicit
