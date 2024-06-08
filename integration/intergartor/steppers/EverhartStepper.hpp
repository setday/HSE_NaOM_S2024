
#pragma once

#include <cstring>
#include <valarray>
#include <vector>

#include "BasicTimeStepper.hpp"

namespace ADAAI::Integration::Integrator::Stepper
{
  template<typename RHS>
  class Everhart_TimeStepper : public TimeStepper<RHS>
  {
  public:
    explicit Everhart_TimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
    }
    const static int                  k = 5;            // never change this number!
    mutable double                    DD[k + 1][k + 1]; // Divided Differences (DD[i][j] = F[t_i, ... , t_j])
    mutable std::array<double, k + 1> F = { 0 };
    // second derivative of y at the points t_0...t_k
    mutable std::array<double, k + 1> B = { 0 };

    void initial_approximation_of_F( double t0, double h ) const
    {
      // F[i] = f(t, y(t), y'(t)) for t = t1, t2, ..., tk
      // Notation: y*(t) := dy(t)/dt
      //           y*(t) = dy_dt[0] + F[0] * (t - t_0)
      //           y(t) = y[0] + dy_dt[0] * (t - t_0) + F_0 * (t - t_0)^2 / 2

      std::vector<std::vector<double>> y( std::vector<double>( RHS::N ), k + 1 );
      std::vector<std::vector<double>> dy_dt( std::vector<double>( RHS::N ), k + 1 );

      // TODO:
      // F_0 = ???
      // y_0 = ???
      // y_dt_0 = ???

      double t0_initial = t0;

      for ( int i = 1; i <= k; i++ )
      {
        // y[i] = y[0] + dy_dt[0] * (t0 - t_initial) + 0.5 * F[0] * (t0 - t_initial)^2
        y[i] = y[0] + dy_dt[0] * ( t0 - t0_initial ) + 0.5 * F[0] * std::pow( t0 - t0_initial, 2 );

        // dy_dt[i] = dy_dt[0] + F[0] * (t0 - t0_initial)
        dy_dt[i] = dy_dt[0] + F[0] * ( t0 - t0_initial );

        // TODO:
        // F[i] = RHS(t0, y[i], dy_dt[i])

        t0 += h;
      }
    }

    void compute_F( double t0, double h ) const
    {
      // y(t) and dy(t)/dt
      std::vector<std::vector<double>> y( std::vector<double>( RHS::N ), k + 1 );
      std::vector<std::vector<double>> dy_dt( std::vector<double>( RHS::N ), k + 1 );

      // Compute y(t) and dy(t)/dt using the formulas provided by L. Merkin
      for ( int i = 0; i <= k; i++ )
      {
        double t = t0 + i * h;

        // (*) dy(t)/dt = [dy(t)/dt]|[t=t0] + sum of B_j * (t - t0) ^ (j + 1) / ( j + 1) over j = 0...k

        for ( int j = 0; j <= k; j++ )
        {
          double coeff = std::pow( t - t0, j + 1 ) / ( j + 1 );
          dy_dt += B[j] * coeff;
        }

        // (**) y(t) = y(t0) + [dy(t)/dt]|[t=t0] * (t - t0) + sum of B_j * (t - t0) ^ (j + 2) / (( j + 1) * (j + 2)) over j = 0...k

        for ( int j = 0; j <= k; j++ )
        {
          double coeff = std::pow( t - t0, j + 2 ) / ( ( j + 1 ) * ( j + 2 ) );
          y += B[j] * coeff;
        }

        // Now we know y(t) and dy(t)/dt at the points, so we can find F

        // TODO:
        // F[i] = RHS(t0, y[i], dy_dt[i])
      }
    }

    /// \brief Computes DD.
    // F[i] must be computed before (using 'initial_approximation_of_F' or 'compute_F')
    void compute_DD( double h ) const
    {
      for ( int i = 0; i <= k - 1; i++ )
      {
        DD[i][i + 1] = ( F[i + 1] - F[i] ) / h;
      }

      for ( int order = 2; order <= k - 1; order++ )
      {
        int vals = k + 1 - order;
        for ( int j = 0; j < vals; j++ )
        {
          DD[j][j + order] = ( DD[j][j + order - 1] - DD[j + 1][j + order - 1] ) / ( order * h );
        }
      }
    }

    //  params are specified only to match the signature of other getBN functions.
    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB0( [[maybe_unused]] double t0, [[maybe_unused]] double h ) const
    {
      return F[0];
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB1( [[maybe_unused]] double t0, double h ) const
    {
      double b1 = 0;
      for ( int i = 1; i <= 5; i++ )
      {
        double prod = 1;
        for ( int j = 1; j <= i - 1; j++ )
        {
          prod *= ( -h * j ); // t_0 - t_j = -h * j
        }
        b1 += DD[0][i] * prod;
      }
      return b1;
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB2( double t0, double h ) const
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      // the following formula was derivided manually and revised in 2 weeks with a help of Wolfram Alpha
      return DD[0][2] + DD[0][3] * ( 2 * t - ( t1 + t2 ) ) + DD[0][4] * ( 3 * t * t - 2 * t * ( t1 + t2 + t3 ) + t1 * ( t2 + t3 ) + t2 * t3 ) + DD[0][5] * ( 4 * t * t * t - 3 * t * t * ( t1 + t2 + t3 + t4 ) + 2 * t * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) - ( t * ( t2 * ( t3 + t4 ) + t3 * t4 ) + t2 * t3 * t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB3( double t0, double h ) const
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      return DD[0][3] * 2 + DD[0][4] * ( 6 * t - 2 * ( t1 + t2 + t3 ) ) + DD[0][5] * ( 12 * t * t - 6 * t * ( t1 + t2 + t3 + t4 ) + 2 * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB4( double t0, double h ) const
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      return DD[0][4] * 6 + DD[0][5] * ( 24 * t - 6 * ( t1 + t2 + t3 + t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB5( [[maybe_unused]] double t0, [[maybe_unused]] double h ) const
    {
      return DD[0][5] * 24;
    }

    /// \brief The stepper function
    /// \param current_time The current time
    /// \param current_state The current state of the system
    /// \param next_state The next state of the system
    /// \return The next time (current_time + dt) and the delta time

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 0.01 ) const override
    {
      double dist_between_adjacent_ts = suggested_d_time / k;

      // Step 0: fix K

      // Step 1: INITIAL APPROXIMATION
      initial_approximation_of_F( current_time, dist_between_adjacent_ts, current_state );

      const int number_of_iterations = 50; // it should be improved later by observing convergence (if almost nothing has changed, then finish the step)

      // Step 2: complete N iterations
      for ( int j = 1; j <= number_of_iterations; j++ )
      {
        // Step 3: compute all divided differences (to order k)
        compute_DD( dist_between_adjacent_ts );

        // Step 4: find B_j (as functions of divided differences)
        B[0] = getB0( current_time, dist_between_adjacent_ts );
        B[1] = getB1( current_time, dist_between_adjacent_ts );
        B[2] = getB2( current_time, dist_between_adjacent_ts );
        B[3] = getB3( current_time, dist_between_adjacent_ts );
        B[4] = getB4( current_time, dist_between_adjacent_ts );
        B[5] = getB5( current_time, dist_between_adjacent_ts );

        // Step 5: compute more accurate y(t), d[y(t)]/dt and F[i]
        compute_F( current_time, dist_between_adjacent_ts );
      }

      // TODO
      // Update the next state with the computed values

      double new_step = 0.01; // we keep it the same.

      return { current_time + suggested_d_time, new_step };
    }
  };
} // namespace ADAAI::Integration::Integrator::Stepper