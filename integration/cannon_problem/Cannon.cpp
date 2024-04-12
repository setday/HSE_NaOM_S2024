#include <iostream>
#include <vector>
#include <tuple>

#include "CannonBall.hpp"

namespace ADAAI::Integration::Cannon
{
  std::pair<double, double> shootWithAngle( double angle = 45 )
  {
    double rad = angle * M_PI / 180.0f;

    double v = 1640.0f; // initial velocity (m/s)

    double state[4] = { 0.0f, 0.0f, v * std::cos( rad ), v * std::sin( rad ) };
    double end_state[4];

    auto rhs      = new CannonBall::BallRHS();
    auto observer = new CannonBall::BallObserver();
    auto stepper  = new Integrator::RFK45_TimeStepper<CannonBall::BallRHS>( rhs );

    auto integrator = Integrator::ODE_Integrator<CannonBall::BallRHS, Integrator::RFK45_TimeStepper<CannonBall::BallRHS>, CannonBall::BallObserver>( stepper, observer );

    double t = integrator( state, end_state );

    return { end_state[0], t };
  }

  /// TODO: Make multy-threading
  double findBestAngle()
  {
    double best_angle    = 0.0f;
    double best_distance = 0.0f;

    const double delta_angle = 0.1f;
    const double min_angle   = 40.0f;
    const double max_angle   = 60.0f;

    std::vector<std::tuple<double, double, double>> results;

    for ( double angle = min_angle; angle < max_angle; angle += delta_angle )
    {
      auto [distance, t] = shootWithAngle( angle );

      results.emplace_back( angle, distance, t );

      if ( distance > best_distance )
      {
        best_distance = distance;
        best_angle    = angle;
      }
    }

    /// TODO: Make file dumping
    for ( auto [angle, distance, t] : results )
    {
      std::cout << "Angle: " << angle << " Distance: " << distance << " Time: " << t << std::endl;
    }

    return best_angle;
  }
} // namespace ADAAI::Integration::Cannon
