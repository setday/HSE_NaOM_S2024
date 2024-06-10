#pragma once

#include "Observer.hpp"
#include "steppers/RFK45_TimeStepper.hpp"

namespace ADAAI::Integration::Integrator
{
  /// \brief The ODE integrator
  /// \tparam RHS The right-hand side of the system of equations
  /// \tparam TS The time stepper
  /// \tparam RHS_O The observer

  template<typename RHS_I = RHS, typename TS = Stepper::RFK45_TimeStepper<RHS_I>, typename RHS_O = Observer<RHS_I>>
    requires std::is_base_of_v<RHS, RHS_I> && std::is_base_of_v<Observer<RHS_I>, RHS_O> && std::is_base_of_v<Stepper::TimeStepper<RHS_I>, TS>
  class ODE_Integrator
  {
    const TS*    m_stepper;
    const RHS_O* m_observer;

  public:
    ODE_Integrator( const TS* stepper, const RHS_O* observer )
        : m_stepper( stepper ), m_observer( observer )
    {
    }

    /// \brief The integrator function
    /// \param state_start The initial state of the system
    /// \param state_end The final state of the system
    /// \param t_start The initial time
    /// \param t_end The final time
    /// \return The time of the final state
    double operator()( const double state_start[TS::N], double state_end[TS::N], double t_start = 0.0, double t_end = 2e3, double suggested_dt = 1e-2 ) const
    {
      double current_time = t_start;
      double current_state[TS::N];
      double next_state[TS::N];

      for ( int i = 0; i < TS::N; ++i )
      {
        current_state[i] = state_start[i];
      }

      int percent_count = 0;

      while ( current_time < t_end )
      {
        if ( !( *m_observer )( current_time, current_state ) )
        {
          break;
        }

        auto [next_time, dt] = ( *m_stepper )( current_state, next_state, current_time, suggested_dt );

        if ( next_time > t_end )
        {
          dt        = t_end - current_time;
          next_time = t_end;
        }

        current_time = next_time;
        for ( int i = 0; i < TS::N; ++i )
        {
          current_state[i] = next_state[i];
        }

        if ( current_time / t_end * 100.0 > percent_count + 5 )
        {
          percent_count += 5;
          std::cout << percent_count << "%" << std::endl;
        }
      }

      for ( int i = 0; i < TS::N; ++i )
      {
        state_end[i] = current_state[i];
      }

      return current_time;
    }
  };
} // namespace ADAAI::Integration::Integrator
