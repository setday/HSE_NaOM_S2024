#pragma once

namespace ADAAI::Integration::Integrator
{
  struct RHS
  {
    constexpr static int N = 0; // The number of equations

    /// \brief The right-hand side of the system of equations
    virtual void operator()( double current_time, const double* current_state, double* rhs ) const = 0;
  };

  struct [[maybe_unused]] HarmonicOsc_RHS : public RHS
  {
  private:
    double const m_omega2;

  public:
    constexpr static int N = 2;

    [[maybe_unused]] explicit HarmonicOsc_RHS( double omega )
        : m_omega2( omega * omega )
    {
    }

    /// \brief The right-hand side of the harmonic oscillator equation
    void operator()( [[maybe_unused]] double current_time, const double* current_state, double* rhs ) const override
    {
      rhs[0] = current_state[1];
      rhs[1] = -m_omega2 * current_state[0];
    }
  }; // struct HarmonicOsc_RHS
} // namespace ADAAI::Integration::Integrator
