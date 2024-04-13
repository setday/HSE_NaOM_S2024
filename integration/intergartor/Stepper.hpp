#pragma once

#include <stdexcept>
#include <utility>

#include "RHS.hpp"

namespace ADAAI::Integration::Integrator
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
  }; // struct Stepper

  template<typename RHS>
  class RFK45_TimeStepper : public TimeStepper<RHS>
  {
  public:
    explicit RFK45_TimeStepper( const RHS* rhs )
        : TimeStepper<RHS>( rhs )
    {
    }

    // ! warning: indexing from 1
    // 0.0 refers to fictive values
    std::vector<double> A_K  = { 0.0, 0, 0.5, 0.5, 2.0 / 3, 0.2 };
    std::vector<double> C_K  = { 0.0, 1.0 / 6, 0, 2.0 / 3, 1.0 / 6 };
    std::vector<double> CH_K = { 0.0, 1.0 / 24, 0, 0, 5.0 / 48, 27.0 / 56, 125.0 / 336 };
    std::vector<double> CT_K = { 0.0, 0.125, 0, 2.0 / 3, 1.0 / 16, -27.0 / 56, -125.0 / 336 };

    std::vector<std::vector<double, double>> B_K_L = {
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
      { 0.0, 0, 0.5, 0.25, 0, 7.0 / 27, 28.0 / 625 },
      { 0.0, 0.0, 0.0, 0.25, -1, 10.0 / 27, -0.2 },
      { 0.0, 0.0, 0.0, 0.0, 2, 0, 546.0 / 625 },
      { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 / 27, 54.0 / 625 },
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0, -378.0 / 625 }
    }

    /// \brief The stepper function
    /// \param current_time The current time
    /// \param current_state The current state of the system
    /// \param next_state The next state of the system
    /// \return The next time (current_time + dt) and the delta time

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 0.01 ) const override
    {
      //      throw std::runtime_error( "Not implemented yet" );

      /// FIXME: just testing function (lines 53-61 should be replaced)
      this->m_rhs->operator()( current_time, current_state, next_state );

      for ( int i = 0; i < RHS::N; ++i )
      {
        next_state[i] = current_state[i];
      }

      return { current_time + suggested_d_time, suggested_d_time };
    }
  };
} // namespace ADAAI::Integration::Integrator
