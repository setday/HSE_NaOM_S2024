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
      if ( current_time <= 1.0 )
        return true;
      return current_state[1] > 100.0; // Optimal to not underground calculation in RHS
    }
  };

  struct BallDumperObserver : Integrator::Observer<BallRHS>
  {
    std::ostream& m_os;

    explicit BallDumperObserver( std::ostream& os )
        : m_os( os )
    {
    }

    bool operator()( double current_time, const double current_state[BallRHS::N] ) const override
    {
      static int ind = 0;

      if ( (ind++) % 10 == 0 )
      {
        try
        {
          double rhs[BallRHS::N] {};
          BallRHS {}( current_time, current_state, rhs );

          m_os << "    {\n"
               << "      \"current_time\":" << current_time << ",\n"
               << "      \"current_state\": {\n"
               << "        \"x\": " << current_state[0] << ",\n"
               << "        \"y\": " << current_state[1] << ",\n"
               << "        \"v_x\": " << current_state[2] << ",\n"
               << "        \"v_y\": " << current_state[3] << "\n"
               << "      },\n"
               << "      \"rhs\": {\n"
               << "        \"v_x\": " << rhs[0] << ",\n"
               << "        \"v_y\": " << rhs[1] << ",\n"
               << "        \"a_x\": " << rhs[2] << ",\n"
               << "        \"a_y\": " << rhs[3] << "\n"
               << "      }\n"
               << "    },\n";
        }
        catch ( ... )
        {
          std::cout << "Error in BallDumperObserver" << std::endl;
        }
      }

      if ( current_time <= 1.0 )
        return true;
      return current_state[1] > 100.0; // Optimal to not underground calculation in RHS
    }
  };
} // namespace ADAAI::Integration::CannonBall
