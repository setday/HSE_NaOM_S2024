
#pragma once

#include <cstring>
#include <iostream>
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

    constexpr static int N2 = RHS::N / 2;
    constexpr static int k  = 5; // never change this number!

    mutable double DD[k + 1][k + 1][N2]; // Divided Differences (DD[i][j] = F[t_i, ... , t_j])
    mutable double F[k + 1][N2];
    // second derivative of y at the points t_0...t_k
    mutable double B[k + 1][N2];

    mutable double y[k + 1][N2];
    mutable double dy_dt[k + 1][N2];

    mutable double state[RHS::N];
    mutable double rhs_out[RHS::N];

    void initial_approximation_of_F( double t0, double h ) const
    {
      // F[i] = f(t, y(t), y'(t)) for t = t1, t2, ..., tk
      // Notation: y*(t) := dy(t)/dt
      //           y*(t) = dy_dt[0] + F[0] * (t - t_0)
      //           y(t) = y[0] + dy_dt[0] * (t - t_0) + F_0 * (t - t_0)^2 / 2


      double t0_initial = t0;

      for ( int i = 0; i <= k; i++ )
      {
        double delt_t = t0 - t0_initial;

        // std::cout << delt_t << '\n';

        // y[i] = y[0] + dy_dt[0] * (t0 - t_initial) + 0.5 * F[0] * (t0 - t_initial)^2
        for ( int equation = 0; equation < N2; equation++ )
        {
          y[i][equation]  = y[0][equation] + dy_dt[0][equation] * delt_t + 0.5 * F[0][equation] * delt_t * delt_t;
          state[equation] = y[i][equation];
          // dy_dt[i] = dy_dt[0] + F[0] * (t0 - t0_initial)
          dy_dt[i][equation]   = dy_dt[0][equation] + F[0][equation] * delt_t;
          state[equation + N2] = dy_dt[i][equation];
        }

        ( *this->m_rhs )( t0, state, rhs_out );

        memcpy( F[i], rhs_out + N2, sizeof( state[0] ) * N2 );

        // std::cout << "x: " << state[0] << " y: " << state[1] << " z: " << state[2] << '\n';
        // std::cout << "v_x: " << state[3] << " v_y: " << state[4] << " v_z: " << state[5] << "\n";
        // std::cout << "a_x: " << (rhs_out + N2)[0] << " a_y: " << (rhs_out + N2)[1] << " a_z: " << (rhs_out + N2)[2] << "\n";
        // std::cout << "a_x: " << F[0][0] << " a_y: " << F[0][1] << " a_z: " << F[0][2] << "\n\n";

        t0 += h;
      }
    }

    void compute_F( double t0, double h ) const
    {
      // y(t) and dy(t)/dt
      double t0_initial = t0;

      // Compute y(t) and dy(t)/dt using the formulas provided by L. Merkin
      for ( int i = 1; i <= k; i++ )
      {
        // (*) dy(t)/dt = [dy(t)/dt]|[t=t0] + sum of B_j * (t - t0) ^ (j + 1) / ( j + 1) over j = 0...k
        t0 += h;

        double delt_t = t0 - t0_initial;

        for ( int equation = 0; equation < N2; equation++ )
        {
          dy_dt[i][equation] = dy_dt[0][equation];
          for ( int j = 0; j <= k; j++ )
          {
            double coeff = std::pow( delt_t, j + 1 ) / ( j + 1 );
            dy_dt[i][equation] += B[j][equation] * coeff;
          }
          state[equation + N2] = dy_dt[i][equation];
        }

        // (**) y(t) = y(t0) + [dy(t)/dt]|[t=t0] * (t - t0) + sum of B_j * (t - t0) ^ (j + 2) / (( j + 1) * (j + 2)) over j = 0...k
        for ( int equation = 0; equation < N2; equation++ )
        {
          y[i][equation] = y[0][equation];
          for ( int j = 0; j <= k; j++ )
          {
            double coeff = std::pow( delt_t, j + 2 ) / ( ( j + 1 ) * ( j + 2 ) );
            y[i][equation] += B[j][equation] * coeff;
          }
          state[equation] = y[i][equation];
        }
        // Now we know y(t) and dy(t)/dt at the points, so we can find F


        ( *this->m_rhs )( t0, state, rhs_out );

        memcpy( F[i], rhs_out + N2, sizeof( state[0] ) * N2 );
      }
    }

    /// \brief Computes DD.
    // F[i] must be computed before (using 'initial_approximation_of_F' or 'compute_F')
    void compute_DD( double h ) const
    {
      for ( int i = 0; i <= k - 1; i++ )
      {
        for ( int index = 0; index < N2; index++ )
        {
          DD[i][i + 1][index] = ( F[i + 1][index] - F[i][index] ) / h;
        }
      }
      for ( int order = 2; order <= k; order++ )
      {
        for ( int index = 0; index < N2; index++ )
        {
          int vals = k + 1 - order;
          for ( int j = 0; j < vals; j++ )
          {
            DD[j][j + order][index] = ( DD[j + 1][j + order][index] - DD[j][j + order - 1][index] ) / ( order * h );
          }
        }
      }
    }

    //  params are specified only to match the signature of other getBN functions.
    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB0( [[maybe_unused]] double t0, [[maybe_unused]] double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        B[0][index] = F[0][index];
      }
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB1( [[maybe_unused]] double t0, double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        double b1 = 0;
        for ( int i = 1; i <= 5; i++ )
        {
          double prod = 1;
          for ( int j = 1; j <= i - 1; j++ )
          {
            prod *= ( -h * j ); // t_0 - t_j = -h * j
          }
          b1 += DD[0][i][index] * prod;
        }
        B[1][index] = b1;
      }
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB2( double t0, double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        double t1 = t0 + h;
        double t2 = t0 + 2 * h;
        double t3 = t0 + 3 * h;
        double t4 = t0 + 4 * h;

        double t = t0;
        // the following formula was derivided manually and revised in 2 weeks with a help of Wolfram Alpha
        B[2][index] = DD[0][2][index] + DD[0][3][index] * ( 2 * t - ( t1 + t2 ) ) + DD[0][4][index] * ( 3 * t * t - 2 * t * ( t1 + t2 + t3 ) + t1 * ( t2 + t3 ) + t2 * t3 ) + DD[0][5][index] * ( 4 * t * t * t - 3 * t * t * ( t1 + t2 + t3 + t4 ) + 2 * t * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) - ( t * ( t2 * ( t3 + t4 ) + t3 * t4 ) + t2 * t3 * t4 ) );
      }
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB3( double t0, double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        double t1 = t0 + h;
        double t2 = t0 + 2 * h;
        double t3 = t0 + 3 * h;
        double t4 = t0 + 4 * h;

        double t    = t0;
        B[3][index] = DD[0][3][index] * 2 + DD[0][4][index] * ( 6 * t - 2 * ( t1 + t2 + t3 ) ) + DD[0][5][index] * ( 12 * t * t - 6 * t * ( t1 + t2 + t3 + t4 ) + 2 * ( t1 * ( t2 + t3 + t4 ) + t2 * ( t3 + t4 ) + t3 * t4 ) );
      }
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB4( double t0, double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        double t1 = t0 + h;
        double t2 = t0 + 2 * h;
        double t3 = t0 + 3 * h;
        double t4 = t0 + 4 * h;

        double t    = t0;
        B[4][index] = DD[0][4][index] * 6 + DD[0][5][index] * ( 24 * t - 6 * ( t1 + t2 + t3 + t4 ) );
      }
    }

    /// \param h  The distance between adjacent ts (e.g. t1-t0)
    void getB5( [[maybe_unused]] double t0, [[maybe_unused]] double h ) const
    {
      for ( int index = 0; index < N2; index++ )
      {
        B[5][index] = DD[0][5][index] * 24;
      }
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

      memcpy( y[0], current_state, sizeof( current_state[0] ) * N2 );
      memcpy( dy_dt[0], current_state + N2, sizeof( current_state[0] ) * N2 );

      // Step 1: INITIAL APPROXIMATION
      initial_approximation_of_F( current_time, dist_between_adjacent_ts );

      const int number_of_iterations = 1; // it should be improved later by observing convergence (if almost nothing has changed, then finish the step)

      // Step 2: complete N iterations
      for ( int j = 1; j <= number_of_iterations; j++ )
      {
        // Step 3: compute all divided differences (to order k)
        compute_DD( dist_between_adjacent_ts );

        // Step 4: find B_j (as functions of divided differences)
        getB0( current_time, dist_between_adjacent_ts );
        getB1( current_time, dist_between_adjacent_ts );
        getB2( current_time, dist_between_adjacent_ts );
        getB3( current_time, dist_between_adjacent_ts );
        getB4( current_time, dist_between_adjacent_ts );
        getB5( current_time, dist_between_adjacent_ts );

        // Step 5: compute more accurate y(t), d[y(t)]/dt and F[i]
        compute_F( current_time, dist_between_adjacent_ts );
      }

      // TODO
      // Update the next state with the computed values

      memcpy( next_state, y[k], sizeof( current_state[0] ) * N2 );
      memcpy( next_state + N2, dy_dt[k], sizeof( current_state[0] ) * N2 );

      return { current_time + suggested_d_time, suggested_d_time };
    }
  };
} // namespace ADAAI::Integration::Integrator::Stepper
