#pragma once

#include "RHS.hpp"

namespace ADAAI::Integration::Integrator
{
  template<typename RHS>
  struct Observer
  {
    virtual bool operator()( double current_time, const double current_state[RHS::N] ) const = 0;
  };
} // namespace ADAAI::Integration::Integrator
