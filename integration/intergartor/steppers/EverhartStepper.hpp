// ======================================================================
// !!! DEV !!! (Nikolay)
// ======================================================================
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
    const static int k = 5;            // never change this number!
    double           DD[k + 1][k + 1]; // Divided Differences (DD[i][j] = F[t_i, ... , t_j])
    double           F[k + 1];         // second derivative of y at the points t_0...t_k

    void initial_approximation_of_F( double t0 = 0, double h = 0.01 )
    {
      // F[i] = f(t, y(t), y'(t)) for t = t1, t2, ..., tk
      for ( int i = 0; i <= k; i++ )
      {
        // TODO
        // F[i] = rhs[t0]
        t0 += h;
      }
    }

    // Computes F using (*) and (**), where (*) and (**) are formulas for computing y(t) and dy(t)/dt (which utilize only B_i coeffs) - see below.
    void compute_F( double t0 = 0, double h = 0 )
    {
      // TODO
      // (*) dy(t)/dt = [dy(t)/dt]|[t=t0] + sum of B_j * (t - t0) ^ (j + 1) / ( j + 1) over j = 0...k

      // TODO
      // (**) y(t) = y(t0) + [dy(t)/dt]|[t=t0] * (t - t0) + sum of B_j * (t - t0) ^ (j + 2) / (( j + 1) * (j + 2)) over j = 0...k

      // TODO
      // Now we know y(t) and dy(t)/dt, so we can find F
    }

    /// \brief Computes DD.
    // F[i] must be computed before (using 'initial_approximation_of_F' or 'compute_F')
    void compute_DD( double t0, double h )
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

    // Default values are used only to match the signature of other getBN functions.
    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB0( double t0 = 0, double h = 0 )
    {
      return F[0];
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB1( double t0, double h )
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
    double getB2( double t0, double h )
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      // the following formula was derivided manually and revised in 2 weeks with a help of Wolfram Alpha
      return DD[0][2] + DD[0][3] * ( 2 * t - ( t1 + t2 ) ) + DD[0][4] * ( 3 * t * t - 2 * t * ( t1 + t2 + t3 ) + t1 * ( t2 + t3 ) + t2 * t3 ) + DD[0][5] * ( 4 * t * t * t - 3 * t * t * ( t1 + t2 + t3 + t4 ) + 2 * t * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) - ( t1( t2 * ( t3 + t4 ) + t3 * t4 ) + t2 * t3 * t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB3( double t0, double h )
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      return DD[0][3] * 2 + DD[0][4] * ( 6 * t - 2 * ( t1 + t2 + t3 ) ) + DD[0][5] * ( 12 * t * t - 6 * t * ( t1 + t2 + t3 + t4 ) + 2 * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB4( double t0, double h )
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      return DD[0][4] * 6 + DD[0][5] * ( 24 * t - 6 * ( t1 + t2 + t3 + t4 ) );
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    double getB5( double t0, double h )
    {
      double t1 = t0 + h;
      double t2 = t0 + 2 * h;
      double t3 = t0 + 3 * h;
      double t4 = t0 + 4 * h;

      double t = t0;
      return DD[0][5] * 24;
    }
    explicit Everhart_TimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
    }

    /// \brief The stepper function
    /// \param current_time The current time
    /// \param current_state The current state of the system
    /// \param next_state The next state of the system
    /// \return The next time (current_time + dt) and the delta time

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 0.01 ) const override
    {
      double x   = current_state[0];
      double y   = current_state[1];
      double z   = current_state[2];
      double v_x = current_state[3];
      double v_y = current_state[5];
      double v_z = current_state[5];

      double dist_between_adjacent_ts = suggested_d_time / k;

      // step 0: fix K
      // Let's say k=5

      // step 1: INITIAL APPROXIMATION
      initial_approximation_of_F( t0 = current_time, h = dist_between_adjacent_ts );

      const int number_of_iterations = 10; // it should be improved later by observing convergence (if almost nothing has changed, then finish the step)

      for ( int z = 1; z <= number_of_iterations; z++ )
      {
        //      step 3: compute all divided differences (to order k)
        compute_DD( current_time, dist_between_adjacent_ts );

        //      step 4: find B_j (as functions of divided differences)
        double b0 = getB0( current_time, dist_between_adjacent_ts ); // I know, it's probably better to use an array for Bs
        double b1 = getB1( current_time, dist_between_adjacent_ts );
        double b2 = getB2( current_time, dist_between_adjacent_ts );
        double b3 = getB3( current_time, dist_between_adjacent_ts );
        double b4 = getB4( current_time, dist_between_adjacent_ts );
        double b5 = getB5( current_time, dist_between_adjacent_ts );

        //      step 5: compute more accurate y(t), d[y(t)]/dt and F[i]
        compute_F( t0 = current_time, h = dist_between_adjacent_ts );
      }

      double new_step = 0.01; // we keep it the same.

      return { current_time + suggested_d_time, new_step };
    }
  };
} // namespace ADAAI::Integration::Integrator::Stepper