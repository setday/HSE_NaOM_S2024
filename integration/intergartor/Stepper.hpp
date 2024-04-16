#pragma once

#include <cmath>
#include <cstring>
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
    std::vector<double> A_K  = { 0.0, 0, 0.5, 0.5, 1, 2.0 / 3, 0.2 };
    std::vector<double> C_K  = { 0.0, 1.0 / 6, 0, 2.0 / 3, 1.0 / 6 };
    std::vector<double> CH_K = { 0.0, 1.0 / 24, 0, 0, 5.0 / 48, 27.0 / 56, 125.0 / 336 };
    std::vector<double> CT_K = { 0.0, 0.125, 0, 2.0 / 3, 1.0 / 16, -27.0 / 56, -125.0 / 336 };

    // B[K][L]
    std::vector<std::vector<double>> B_K_L = {
        { 0.0,          0.0,         0.0,           0.0,          0.0,            0.0 },
        { 0.0,          0.0,         0.0,           0.0,          0.0,            0.0 },
        { 0.0,          0.5,         0.0,           0.0,          0.0,            0.0 }, // K = 2
        { 0.0,         0.25,        0.25,           0.0,          0.0,            0.0 }, // K = 3
        { 0.0,          0.0,        -1.0,           2.0,          0.0,            0.0 }, // K = 4
        { 0.0,   7.0 / 27.0, 10.0 / 27.0,           0.0,   1.0 / 27.0,            0.0 }, // K = 5
        { 0.0, 28.0 / 625.0,        -0.2, 546.0 / 625.0, 54.0 / 625.0, -378.0 / 625.0 }, // K = 6
    };

    /// \brief The stepper function
    /// \param current_time The current time
    /// \param current_state The current state of the system
    /// \param next_state The next state of the system
    /// \return The next time (current_time + dt) and the delta time

    std::pair<double, double>
    operator()( double current_state[RHS::N], double next_state[RHS::N], double current_time, double suggested_d_time = 0.01 ) const override
    {
      // ======================================================================
      // utils
      double buf[RHS::N];
      double cur[RHS::N];
      double h = suggested_d_time;

      auto mul = []( double* data, double c, std::size_t size )
      {
        for ( std::size_t i = 0; i < size; ++i )
        {
          data[i] *= c;
        }
      };

      auto add = []( double* lhs, double* rhs, std::size_t size )
      {
        for ( std::size_t i = 0; i < size; ++i )
        {
          lhs[i] += rhs[i];
        }
      };

      // ======================================================================
      // find ks
      std::vector<std::vector<double>> ks( 6, std::vector<double>( RHS::N ) );
      for ( std::size_t i = 0; i < ks.size(); ++i )
      {
        std::memcpy( cur, current_state, RHS::N * sizeof( double ) );
        for ( std::size_t j = 0; j < i; ++j )
        {
          std::memcpy( buf, ks[j].data(), RHS::N * sizeof( double ) );
          mul( buf, B_K_L[i + 1][j + 1], RHS::N );
          add( cur, buf, RHS::N );
        }
        ( *this->m_rhs )( current_time + A_K[i + 1] * h, cur, ks[i].data() );
        mul( ks[i].data(), h, RHS::N );
      }

      // ======================================================================
      // find error
      std::memset( cur, 0, RHS::N * sizeof( double ) );
      for ( std::size_t i = 0; i < ks.size(); ++i )
      {
        std::memcpy( buf, ks[i].data(), RHS::N * sizeof( double ) );
        mul( buf, CT_K[i + 1], RHS::N );
        add( cur, buf, RHS::N );
      }
      double TE = 0;
      for ( int i = 0; i < RHS::N; ++i )
      {
        TE += cur[i] * cur[i];
      }
      double eps = 1e-9;
      // what eps to choose?
      double new_step = 0.9 * h * std::pow( eps / TE, 0.1 );
      if(TE > eps) {
        return (*this)(current_state, next_state, current_time, new_step);
      }

      // ======================================================================
      // save res
      std::memcpy( next_state, current_state, RHS::N * sizeof( double ) );
      for ( int i = 0; i < RHS::N; ++i )
      {
        std::memcpy( buf, ks[i].data(), RHS::N * sizeof( double ) );
        mul( buf, CH_K[i + 1], RHS::N );
        add( next_state, buf, RHS::N );
      }

      return { current_time + h, new_step };
    }
  };
} // namespace ADAAI::Integration::Integrator
