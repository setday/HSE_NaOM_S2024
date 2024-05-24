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
    explicit Everhart_TimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
    }
    const static int          k = 5;            // never change this number!
    double                    DD[k + 1][k + 1]; // Divided Differences (DD[i][j] = F[t_i, ... , t_j])
    std::array<double, k + 1> F = { 0 };
    // second derivative of y at the points t_0...t_k
    std::array<double, k + 1> B = { 0 };

    void initial_approximation_of_F( double t0 = 0, double h = 0.01 )
    {
      // F[i] = f(t, y(t), y'(t)) for t = t1, t2, ..., tk
      // Notation: y*(t) := dy(t)/dt
      //           y*(t) = y_dt[0] + F[0] * (t - t_0)
      //           y(t) = y[0] + y_dt[0] * (t - t_0) + F_0 * (t - t_0)^2 / 2

      std::vector<std::valarray<double>> y( k + 1, std::valarray<double>( 3 ) );
      std::vector<std::valarray<double>> y_dt( k + 1, std::valarray<double>( 3 ) );

      // Assuming current_state contains the initial state of the system:
      // current_state[0] = x, current_state[1] = y, current_state[2] = z
      // current_state[3] = v_x, current_state[4] = v_y, current_state[5] = v_z
      const double* current_state = this->rhs->current_state;

      // Initialize y[0] with the initial position
      y[0][0] = current_state[0]; // x
      y[0][1] = current_state[1]; // y
      y[0][2] = current_state[2]; // z

      // Initialize y_dt[0] with the initial velocity
      y_dt[0][0] = current_state[3]; // v_x
      y_dt[0][1] = current_state[4]; // v_y
      y_dt[0][2] = current_state[5]; // v_z

      // Initialize t0_initial for calculating future positions and velocities
      double t0_initial = t0;

      for ( int i = 1; i <= k; i++ )
      {
        // y[i] = y[0] + y_dt[0] * (t0 - t_initial) + 0.5 * F[0] * (t0 - t_initial)^2
        y[i] = y[0] + y_dt[0] * ( t0 - t0_initial ) + 0.5 * F[0] * std::pow( t0 - t0_initial, 2 );

        // y_dt[i] = y_dt[0] + F[0] * (t0 - t0_initial)
        y_dt[i] = y_dt[0] + F[0] * ( t0 - t0_initial );

        // F[i] = RHS(t0, y[i], y_dt[i])
        std::valarray<double> rhs( 6 );
        this->rhs->           operator()( t0, &y[i][0], &rhs[0] );
        F[i] = rhs[3]; // rhs[3] corresponds to the acceleration in the x direction

        t0 += h;
      }
    }

    void compute_F( double t0 = 0, double h = 0 )
    {
      // The number of dimensions for the state vector (3D space)
      constexpr int dim = 3;

      // Vectors to store y(t) and dy(t)/dt
      std::valarray<double> y( dim );
      std::valarray<double> y_dt( dim );

      // Compute y(t) and dy(t)/dt using the formulas provided by L. Merkin
      for ( int i = 0; i <= k; i++ )
      {
        double t = t0 + i * h;

        // (*) dy(t)/dt = [dy(t)/dt]|[t=t0] + sum of B_j * (t - t0) ^ (j + 1) / ( j + 1) over j = 0...k
        std::valarray<double> dy_dt = y_dt[0];
        for ( int j = 0; j <= k; j++ )
        {
          double coeff = std::pow( t - t0, j + 1 ) / ( j + 1 );
          dy_dt += B[j] * coeff;
        }

        // (**) y(t) = y(t0) + [dy(t)/dt]|[t=t0] * (t - t0) + sum of B_j * (t - t0) ^ (j + 2) / (( j + 1) * (j + 2)) over j = 0...k
        std::valarray<double> y_val = y[0] + y_dt[0] * ( t - t0 );
        for ( int j = 0; j <= k; j++ )
        {
          double coeff = std::pow( t - t0, j + 2 ) / ( ( j + 1 ) * ( j + 2 ) );
          y_val += B[j] * coeff;
        }

        // Now we know y(t) and dy(t)/dt at the points, so we can find F
        double current_state[dim * 2];
        std::copy( std::begin( y_val ), std::end( y_val ), current_state );
        std::copy( std::begin( dy_dt ), std::end( dy_dt ), current_state + dim );

        std::valarray<double> rhs( dim * 2 );
        this->rhs->           operator()( t, current_state, &rhs[0] );
        F[i] = rhs[3]; // Update F[i] based on the new state
      }
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