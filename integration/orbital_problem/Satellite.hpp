#pragma once

#include "../environment/ComputeFunctions.hpp"
#include "../intergartor/Observer.hpp"

/// Integrator implementation for the satellite launcher

namespace ADAAI::Integration::Satellite
{
  struct SatelliteRHS : Integrator::RHS
  {
  private:
    // float x;
    // float y;
    // float z;
    // float v_x;
    // float v_y;
    // float v_z;

  public:
    constexpr static int N = 6; // x, y, z, v_x, v_y, v_z

    SatelliteRHS() = default;

    void operator()( double current_time, const double* current_state, double* rhs ) const override
    {
      rhs[0] = current_state[3];
      rhs[1] = current_state[4];
      rhs[2] = current_state[5];

      Environment::ComputeUGradient( current_state, rhs + 3 );
    }
  };

  struct SatelliteObserver : Integrator::Observer<SatelliteRHS>
  {
    bool operator()( double current_time, const double current_state[SatelliteRHS::N] ) const override
    {
      return current_time <= 3.1e7; // 1 year
    }
  };

  struct SatelliteDumperObserver : Integrator::Observer<SatelliteRHS>
  {
    std::ostream& m_os;

    explicit SatelliteDumperObserver( std::ostream& os )
        : m_os( os )
    {
    }

    bool operator()( double current_time, const double current_state[SatelliteRHS::N] ) const override
    {
      static int ind = 0;

      if ( ( ind++ ) % 10 == 0 )
      {
        try
        {
          double rhs[SatelliteRHS::N] {};
          SatelliteRHS {}( current_time, current_state, rhs );

          m_os << "  {\n"
               << "    \"current_time\":" << current_time << ",\n"
               << "    \"current_state\": {\n"
               << "      \"x\": " << current_state[0] << ",\n"
               << "      \"y\": " << current_state[1] << ",\n"
               << "      \"z\": " << current_state[2] << ",\n"
               << "      \"v_x\": " << current_state[2] << ",\n"
               << "      \"v_y\": " << current_state[3] << ",\n"
               << "      \"v_z\": " << current_state[4] << ",\n"
               << "      \"a_x\": " << rhs[2] << ",\n"
               << "      \"a_y\": " << rhs[3] << ",\n"
               << "      \"a_z\": " << rhs[4] << "\n"
               << "    }\n"
               << "  },\n";
        }
        catch ( ... )
        {
          std::cout << "Error in BallDumperObserver" << std::endl;
        }
      }

      return current_time <= 3.1e7; // 1 year
    }
  };
} // namespace ADAAI::Integration::Satellite
