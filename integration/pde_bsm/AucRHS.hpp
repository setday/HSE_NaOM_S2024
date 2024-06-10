#pragma once

#include "../intergartor/Observer.hpp"
#include "AuxiliaryFunctions.hpp"

namespace ADAAI::Integration::PDE_BSM
{
  struct AucRHS : Integrator::RHS
  {
    constexpr static int N = 502; // Number of equations

    constexpr static double tau_max = 1.0; // Tau Maxim, Nikolay said
    constexpr static int K = 100;          // Strike price
    constexpr static double S_max = K * std::exp( 5 * AUX_FUNC::sigma_max );

    // double S[ N ]; // Stock price

    AucRHS() {
      // for ( int i = 0; i < N; i++ )
      // {
      //   S[ i ] = i * S_max / N;
      // }
    }

    void operator()( double current_time, const double* current_state, double* rhs ) const override
    {
      double sigma_tau = AUX_FUNC::sigma_function( current_time );
      double sigma_tau2 = sigma_tau * sigma_tau;

      for ( int i = 1; i < N - 1; ++i )
      {
        double prev_c = current_state[ i - 1 ];
        double curr_c = current_state[ i ];
        double next_c = current_state[ i + 1 ];

        if ( i == 1 )
        {
          prev_c = 0;
        }
        if ( i == N - 2 )
        {
          next_c = S_max - K * std::exp( -AUX_FUNC::get_r_integral( current_time ) );
        }

        rhs[ i ] =
            AUX_FUNC::risk_free_interest_rate_function( current_time ) * i * ( next_c - prev_c ) / 2.0 +
            sigma_tau2 * i * i * ( next_c - 2.0 * curr_c + prev_c ) / 2.0 -
            AUX_FUNC::risk_free_interest_rate_function( current_time ) * curr_c;
      }
    }
  };

  struct AucFunc {
    static double get_c( double* state, double S_tau )
    {
      int i = 0;
      while ( i * AucRHS::S_max / AucRHS::N <= S_tau )
      {
        i++;
      }

      i--;
      S_tau -= i * AucRHS::S_max / AucRHS::N;

      return state[ i ] * (1.0 - S_tau) + state[ i + 1 ] * S_tau;
    }

    static void initStartCondition( double* state )
    {
      for ( int i = 0; i < AucRHS::N; i++ )
      {
        state[ i ] = std::max( i * AucRHS::S_max / AucRHS::N - AucRHS::K, 0.0 );
      }
    }
  };

  struct AucObserver : Integrator::Observer<AucRHS>
  {
    explicit AucObserver() {}

    bool operator()( double current_time, const double current_state[AucRHS::N] ) const override
    {
      return current_time < 1.0;
    }
  };
} // namespace ADAAI::Integration::PDE_BSM
