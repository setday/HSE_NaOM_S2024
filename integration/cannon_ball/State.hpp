#pragma once

#include "../environment/ComputeFunctions.hpp"

namespace ADAAI::Integration::CannonBall
{
  struct State {
    float x;
    float y;
    float v_x;
    float v_y;

    float m = 106.0f; // mass of the cannonball (kg)

    State( float x, float y, float v_x, float v_y )
        : x( x ), y( y ), v_x( v_x ), v_y( v_y )
    {
    }

    [[nodiscard]] State get_derivative( float t ) const
    {
      float v2 = v_x * v_x + v_y * v_y;
      float v = std::sqrt( v2 );

      return { v_x, v_y, -Environment::AeroDynamicForce( y, v2 ) * v_x / v / m, -Environment::AeroDynamicForce( y, v2 ) * v_y / v / m - Environment::G_force };
    }
  };
} // namespace ADAAI::Integration::CannonBall

namespace ADAAI::Integration::Cannon {
  float shootWithAngle( float angle = 45 )
  {
    float rad = angle * M_PI / 180.0f;

    float v = 1640.0f; // initial velocity (m/s)

    CannonBall::State state( 0.0f, 0.0f, v * std::cos( rad ), v * std::sin( rad ) );

    /// TODO: Implement the Runge-Kutta method here

    return state.x;
  }
}
