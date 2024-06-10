#include "../AuxiliaryFunctions.hpp"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

namespace ADAAI::Integration::PDE_BSM::Implicit
{
  constexpr static uint64_t N = 100;
  constexpr static uint64_t M = 100;
  constexpr static uint64_t K = 100;

  constexpr static double TAU_MAX = 1.0;
  constexpr static double DT      = TAU_MAX / N;

  constexpr static double S_MAX = K * std::exp( 5 * AUX_FUNC::sigma_max );
  constexpr static double DS    = S_MAX / N;

  constexpr static double S_TAU_MAX = 0.9 * K;

  double tau_j( uint64_t j )
  {
    return j * TAU_MAX / M;
  }

  double s_i( uint64_t i )
  {
    return i * DS;
  }

  void init_c_ij( std::vector<std::vector<double>>& c_ij )
  {
    for ( uint64_t j = 0; j <= M; ++j )
    {
      c_ij[0][j] = 0.0;
      c_ij[N][j] = S_MAX - K * std::exp( -AUX_FUNC::get_r_integral( tau_j( j ) ) );
    }
    for ( uint64_t i = 0; i <= N; ++i )
    {
      c_ij[i][0] = std::max( i * S_MAX / N - K, 0.0 );
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

  void init_matrix( std::vector<std::vector<double>>& matrix, uint64_t j )
  {
    for ( uint64_t i = 1; i <= N - 2; ++i )
    {
      matrix[i][i]     = B_ij( i, j );
      matrix[i + 1][i] = A_ij( i + 1, j );
      matrix[i][i + 1] = D_ij( i, j );
    }
    matrix[N - 1][N - 1] = B_ij( N - 1, j );
  }

  void runSolution( void )
  {
    std::vector<std::vector<double>> c_ij( N + 1, std::vector<double>( M + 1 ) );
    init_c_ij( c_ij );

    std::vector<std::vector<double>> matrix( N, std::vector<double>( N ) );
    std::vector<double>              F_i( N );
    for ( uint64_t j = 1; j <= M; ++j )
    {
      init_matrix( matrix, j );

      // itit F_i
      for ( uint64_t i = 1; i <= N - 1; ++i )
      {
        F_i[i] = -c_ij[i][j - 1] / DT;
      }
      F_i[1] -= A_ij( 1, j ) * c_ij[0][j];
      F_i[N - 1] -= D_ij( N - 1, j ) * c_ij[N][j];

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

      for ( uint64_t i = 1; i <= N - 1; ++i )
      {
        c_ij[i][j] = F_i[i] / matrix[i][i];
      }
    }

    // for(uint64_t i = 0; i <= N; ++i) {
    //     for(uint64_t j = 0; j <= M; ++j) {
    //         std::printf("%.2g ", c_ij[i][j]);
    //     }
    //     std::printf("\n");
    // }

    uint64_t i = 0;
    while ( s_i( i ) <= S_TAU_MAX )
    {
      ++i;
    }
    --i;
    double S_tau = S_TAU_MAX - i * S_MAX / N;

    std::cout << c_ij[i][N] * ( 1.0 - S_tau ) + c_ij[i][N] * S_tau;
  }
} // namespace ADAAI::Integration::PDE_BSM::Implicit
