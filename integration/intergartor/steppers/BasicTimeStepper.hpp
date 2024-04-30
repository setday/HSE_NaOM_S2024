#pragma once

#include "../RHS.hpp"

namespace ADAAI::Integration::Integrator::Stepper
{
  template<typename RHS>
  class TimeStepper
  {
  protected:
    const RHS* m_rhs;

  public:
    constexpr static int N = RHS::N;

    explicit TimeStepper( const RHS* rhs )
        : m_rhs( rhs )
    {
    }

    /// \brief The stepper function
    /// \param current_time The current time
    /// \param current_state The current state of the system
    /// \param next_state The next state of the system
    /// \return The next time (current_time + dt) and the delta time

    virtual std::pair<double, double>
    operator()( double current_state[N], double next_state[N], double current_time, double suggested_d_time ) const = 0;
  }; // class Stepper

  template<typename RHS>
  class DiscreteTimeStepper : public TimeStepper<RHS>
  {
  public:
    explicit DiscreteTimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
    }

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 1e-2 ) const override
    {
      double rhs[RHS::N] {};
      ( *this->m_rhs )( current_time, current_state, rhs );

      for ( int i = 0; i < RHS::N; ++i )
        next_state[i] = current_state[i] + suggested_d_time * rhs[i];

      return { current_time + suggested_d_time, suggested_d_time };
    }
  }; // class DiscreteTimeStepper

  template<typename RHS>
  class GSLTimeStepper : public TimeStepper<RHS>
  {
    // gsl_odeiv2_driver* m_driver;

  public:
    explicit GSLTimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
      // gsl_odeiv2_system system = { fakeRHS, nullptr, RHS::N, nullptr };
      // m_driver = gsl_odeiv2_driver_alloc_y_new( &system, &gsl_odeiv2_step_rkf45, 1e-2, 1e-4, 1e-4 );

      throw std::runtime_error( "I don't have GSL installed, so this code is not implemented :(\n\nPS: Very typical error in production code" );
    }

    int fakeRHS( double t, const double y[], double f[], void* params )
    {
      ( *this->m_rhs )( t, y, f );

      // return GSL_SUCCESS;
      return 0;
    }

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 1e-2 ) const override
    {
      double current_time_copy = current_time;
      // int status = gsl_odeiv2_driver_apply( m_driver, &current_time, current_time + suggested_d_time, current_state );

      // if ( status != GSL_SUCCESS )
      // {
      //   throw std::runtime_error( "GSL integration failed" );
      // }

      // return { current_time, current_time - current_time_copy };

      throw std::runtime_error( "Still not implemented :(" );
    }
  }; // class GSLTimeStepper
} // namespace ADAAI::Integration::Integrator::Stepper
