#include <cmath>
#include <cstdint>

#include "../../intergartor/steppers/BasicTimeStepper.hpp"
#include "../AuxiliaryFunctions.hpp"

namespace ADAAI::Integration::PDE_BSM::Implicit
{
  template<typename RHS>
  class ImplicitStepper : public Integration::Integrator::Stepper::TimeStepper<RHS>
  {
  public:
    constexpr static int N = RHS::N - 1;

  private:
    double A_ij( uint64_t i, double tau, [[maybe_unused]] double dTau ) const
    {
      double a = i * AUX_FUNC::risk_free_interest_rate_function( tau );
      double b = i * AUX_FUNC::sigma_function( tau );

      return b * b / 2 - a;
    }

    double B_ij( uint64_t i, double tau, double dTau ) const
    {
      double b = i * AUX_FUNC::sigma_function( tau );
      double c = -AUX_FUNC::risk_free_interest_rate_function( tau );

      return c - b * b - 1 / dTau;
    }

    double D_ij( uint64_t i, double tau, [[maybe_unused]] double dTau ) const
    {
      double a = i * AUX_FUNC::risk_free_interest_rate_function( tau );
      double b = i * AUX_FUNC::sigma_function( tau );

      return b * b / 2 + a;
    }

    void init_matrix( double matrix[N][N], double tau, double dTau ) const
    {
      for ( int i = 1; i <= N - 2; ++i )
      {
        matrix[i][i]     = B_ij( i, tau, dTau );
        matrix[i + 1][i] = A_ij( i + 1, tau, dTau );
        matrix[i][i + 1] = D_ij( i, tau, dTau );
      }
      matrix[N - 1][N - 1] = B_ij( N - 1, tau, dTau );
    }

    double get_zero_state() const
    {
      return 0.0;
    }

    double get_last_state( double tau ) const
    {
      return AucRHS::S_max - AucRHS::K * std::exp( -AUX_FUNC::get_r_integral( tau ) );
    }

    void init_F_i( double F_i[N], double matrix[N][N], double current_state[RHS::N], double tau, double dTau ) const
    {
      for ( uint64_t i = 1; i <= N - 1; ++i )
      {
        F_i[i] = -current_state[i] / dTau;
      }

      F_i[1] -= A_ij( 1, tau, dTau ) * get_zero_state();
      F_i[N - 1] -= D_ij( N - 1, tau, dTau ) * get_last_state( tau );

      for ( int i = 1; i <= N - 2; ++i )
      {
        double coef      = matrix[i + 1][i] / matrix[i][i];
        matrix[i + 1][i] = 0.0;
        matrix[i + 1][i + 1] -= coef * matrix[i][i + 1];

        F_i[i + 1] -= coef * F_i[i];
      }

      for ( int i = N - 1; i >= 2; --i )
      {
        double coef      = matrix[i - 1][i] / matrix[i][i];
        matrix[i - 1][i] = 0.0;

        F_i[i - 1] -= coef * F_i[i];
      }
    }

    void recalculate_state( double F_i[N], double matrix[N][N], double next_state[RHS::N], double tau ) const
    {
      for ( int i = 1; i <= N - 1; ++i )
      {
        next_state[i] = F_i[i] / matrix[i][i];
      }

      next_state[0] = get_zero_state();
      next_state[N] = get_last_state( tau );
    }

  public:
    explicit ImplicitStepper( RHS* rhs )
        : Integration::Integrator::Stepper::TimeStepper<RHS>( rhs )
    {
    }

    std::pair<double, double> operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time ) const override
    {
      static double matrix[N][N];
      static double F_i[N];

      init_matrix( matrix, current_time, suggested_d_time );
      init_F_i( F_i, matrix, current_state, current_time, suggested_d_time );

      recalculate_state( F_i, matrix, next_state, current_time );

      return { current_time + suggested_d_time, suggested_d_time };
    }
  };
} // namespace ADAAI::Integration::PDE_BSM::Implicit
