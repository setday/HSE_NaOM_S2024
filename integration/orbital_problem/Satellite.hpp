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

    void operator()( [[maybe_unused]] double current_time, const double* current_state, double* rhs ) const override
    {
      rhs[0] = current_state[3];
      rhs[1] = current_state[4];
      rhs[2] = current_state[5];

      Environment::ComputeUGradient( current_state, rhs + 3 );
    }
  };

  struct SatelliteObserver : Integrator::Observer<SatelliteRHS>
  {
    bool operator()( double current_time, [[maybe_unused]] const double current_state[SatelliteRHS::N] ) const override
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

      double rhs[SatelliteRHS::N] {};
      SatelliteRHS {}( current_time, current_state, rhs );

      double
          x = current_state[0],
          y = current_state[1],
          z = current_state[2];
      double
          v_x = current_state[3],
          v_y = current_state[4],
          v_z = current_state[5];
      double
          a_x = rhs[3],
          a_y = rhs[4],
          a_z = rhs[5];

      if ( ( ind++ ) % 10 == 0 )
      {
        m_os << "  {\n"
             << "    \"current_time\":" << current_time << ",\n"
             << "    \"current_state\": {\n"
             << "      \"x\": " << x << ",\n"
             << "      \"y\": " << y << ",\n"
             << "      \"z\": " << z << ",\n"
             << "      \"v_x\": " << v_x << ",\n"
             << "      \"v_y\": " << v_y << ",\n"
             << "      \"v_z\": " << v_z << ",\n"
             << "      \"a_x\": " << a_x << ",\n"
             << "      \"a_y\": " << a_y << ",\n"
             << "      \"a_z\": " << a_z << "\n"
             << "    }\n"
             << "  },\n";
      }

      double
          r2 = x * x + y * y + z * z;

      return current_time <= 3.1e7 && r2 > 8000; // 1 year
    }
  };
} // namespace ADAAI::Integration::Satellite
