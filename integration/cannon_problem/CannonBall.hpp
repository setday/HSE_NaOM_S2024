#pragma once

#include "../environment/ComputeFunctions.hpp"
#include "../intergartor/Interator.hpp"

/// Integrator implementation for the cannonball problem

namespace ADAAI::Integration::CannonBall
{
  struct BallRHS : Integrator::RHS
  {
  private:
    float m = 106.0f; // mass of the cannonball (kg)

    // float x;
    // float y;
    // float v_x;
    // float v_y;

  public:
    constexpr static int N = 4; // x, y, v_x, v_y

    BallRHS() = default;

    void operator()( double current_time, const double* current_state, double* rhs ) const override
    {
      double
          x   = current_state[0],
          y   = current_state[1],
          v_x = current_state[2],
          v_y = current_state[3];

      double v2 = v_x * v_x + v_y * v_y;
      double v  = std::sqrt( v2 );

      rhs[0] = v_x;
      rhs[1] = v_y;
      rhs[2] = -Environment::AeroDynamicForce( y, v2 ) * v_x / v / m;
      rhs[3] = -Environment::AeroDynamicForce( y, v2 ) * v_y / v / m - Environment::G_force;
    }
  };

  struct BallObserver : Integrator::Observer<BallRHS>
  {
    bool operator()( double current_time, const double current_state[BallRHS::N] ) const override
    {
      if ( current_time == 0 )
        return true;
      return current_state[1] > 0.0;
    }
  };
} // namespace ADAAI::Integration::CannonBall
