#include <fstream>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

#include "CannonBall.hpp"

namespace ADAAI::Integration::Cannon
{
  std::pair<double, double> shootWithAngle( double angle = 45 )
  {
    double rad = angle * M_PI / 180.0f;

    double v = 1640.0f; // initial velocity (m/s)

    double state[4] = { 0.0f, 0.0f, v * std::cos( rad ), v * std::sin( rad ) };
    double end_state[4];

    auto rhs      = CannonBall::BallRHS();
    auto observer = CannonBall::BallObserver();
    auto stepper  = Integrator::RFK45_TimeStepper( &rhs );

    auto integrator = Integrator::ODE_Integrator<CannonBall::BallRHS>( &stepper, &observer );

    double t = 0.0;
    try {
      t = integrator( state, end_state );
    } catch ( std::exception& e ) {
      std::cerr << e.what() << std::endl;
    }

    return { end_state[0], t };
  }

  void shootWithAngle( std::ostream& os, double angle = 45 )
  {
    double rad = angle * M_PI / 180.0f;

    double v = 1640.0f; // initial velocity (m/s)

    double state[4] = { 0.0f, 0.0f, v * std::cos( rad ), v * std::sin( rad ) };
    double end_state[4];

    auto rhs      = CannonBall::BallRHS();
    auto observer = CannonBall::BallDumperObserver( os );
    auto stepper  = Integrator::RFK45_TimeStepper( &rhs );

    auto integrator = Integrator::ODE_Integrator<CannonBall::BallRHS>( &stepper, &observer );

    try {
      integrator( state, end_state );
    } catch ( std::exception& e ) {
      std::cerr << e.what() << std::endl;
    }
  }

  void checkRange( std::vector<std::tuple<double, double, double>>* results, double min_angle, double max_angle, double delta_angle )
  {
    for ( double angle = min_angle; angle < max_angle; angle += delta_angle )
    {
      auto [distance, t] = shootWithAngle( angle );

      results->emplace_back( angle, distance, t );
    }
  }

  double findBestAngle()
  {
    const int thread_cnt = ( int ) std::thread::hardware_concurrency() - 1;

    const double delta_angle = 1;
    const double min_angle   = 40.0;
    const double max_angle   = 60.0;
    const int    cnt_angle   = ( int ) ( ( max_angle - min_angle ) / delta_angle );
    const double td_angle    = ( cnt_angle / thread_cnt ) * delta_angle;

    // For initialization purposes (in multy-threading it could be very useful)
    Environment::DrugCoefficient( 1.3 );

    std::vector<std::vector<std::tuple<double, double, double>>> results( thread_cnt );
    std::vector<std::thread>                                     threads;

    for ( int i = 0; i < thread_cnt; i++ )
    {
      double l_angle = min_angle + td_angle * ( double ) i;
      double r_angle = i != thread_cnt - 1 ? l_angle + td_angle : max_angle;

      // checkRange( &results[i], l_angle, r_angle, delta_angle );
      threads.emplace_back( checkRange, &results[i], l_angle, r_angle, delta_angle );
    }

    for ( auto& th : threads )
    {
      th.join();
    }

    double best_angle    = min_angle;
    double best_distance = 0.0f;

    std::ofstream file;
    file.open( "./../data/Cannon_angle_results.data", std::ios::out | std::ios::trunc );

    for ( const auto& result_block : results )
    {
      for ( auto [angle, distance, t] : result_block )
      {
        if ( t != 0.0 )
        {
          file << "Angle: " << angle << " Distance: " << distance << " Time: " << t << '\n';
          std::cout << "Angle: " << angle << " Distance: " << distance << " Time: " << t << std::endl;
        }
        else
        {
          file << "Angle: " << angle << " ==>Error<==" << '\n';
          std::cout << "Angle: " << angle << " ==>Error<==" << std::endl;
        }

        if ( distance > best_distance )
        {
          best_distance = distance;
          best_angle    = angle;
        }
      }
    }

    file << "\nOverall ======================================\n";
    file << "Best angle: " << best_angle << " Best distance: " << best_distance << '\n';
    file.close();

    // Dump best trajectory
    file.open( "./../data/Cannon_best_angle_data.json", std::ios::out );
    file << "{\n";
    file << "  \"angle\": " << best_angle << ",\n";
    file << "  \"distance\": " << best_distance << ",\n";
    file << "  \"data\": [\n";

    shootWithAngle( file, best_angle );

    file << "  ]\n";
    file << "}\n";
    file.close();

    return best_angle;
  }
} // namespace ADAAI::Integration::Cannon
