#include <fstream>
#include <iostream>
#include <vector>

#include "../intergartor/Interator.hpp"
#include "Satellite.hpp"

namespace ADAAI::Integration::Satellite
{
  constexpr static double Altitude = 7500.0f;                    // Altitude of the satellite (km). From center of the Earth
  constexpr static double V02      = Environment::Mu / Altitude; // Initial velocity (km/s)

  void launchSatellite()
  {
    std::ofstream file;
    file.open( "./../data/Satellite_trajectory.json", std::ios::out | std::ios::trunc );
    file << "[\n";

    double state[6] = { 0.0f, 0.0f, Altitude, std::sqrt( V02 ), 0.0f, 0.0f };
    double end_state[6];

    auto rhs      = SatelliteRHS();
    auto observer = SatelliteDumperObserver( file );
    auto stepper  = Integrator::Stepper::DiscreteTimeStepper( &rhs );

    auto integrator = Integrator::ODE_Integrator<SatelliteRHS, Integrator::Stepper::DiscreteTimeStepper<SatelliteRHS>>( &stepper, &observer );

    double t = 0.0;
    try
    {
      t = integrator( state, end_state, 0.0, 4e7, 3.0 );
    }
    catch ( std::exception& e )
    {
      std::cerr << e.what() << std::endl;
    }

    file << "  {\n"
         << "    \"current_time\": " << t << ",\n"
         << "    \"current_state\": {\n"
         << "      \"x\": " << end_state[0] << ",\n"
         << "      \"y\": " << end_state[1] << ",\n"
         << "      \"z\": " << end_state[2] << ",\n"
         << "      \"v_x\": " << end_state[3] << ",\n"
         << "      \"v_y\": " << end_state[4] << ",\n"
         << "      \"v_z\": " << end_state[5] << "\n"
         << "    }\n"
         << "  }\n";
    file << "]\n";
    file.close();
  }
} // namespace ADAAI::Integration::Satellite
